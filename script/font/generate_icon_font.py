#!/usr/bin/env python3
"""
Bitwig Icon Font Generator

Converts SVG icons to TTF font + C++ header for LVGL.
Called by: generate_bitwig_icons.sh
Config via environment: SVG_SOURCE_DIR, TTF_OUTPUT, HEADER_OUTPUT
Output: TTF file, C++ header, unicode range on stdout
"""

from __future__ import annotations

import os
import re
import sys
from datetime import datetime
from pathlib import Path
from typing import Any

try:
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    import svgpathtools
except ImportError as e:
    print(f"error: {e}\nrun: uv sync", file=sys.stderr)
    sys.exit(1)

# --- Config from environment ---
def get_env(name: str, default: str = "") -> str:
    val = os.environ.get(name, default)
    if not val and not default:
        print(f"error: {name} not set", file=sys.stderr)
        sys.exit(1)
    return val

SVG_SOURCE_DIR = Path(get_env("SVG_SOURCE_DIR"))
TTF_OUTPUT = Path(get_env("TTF_OUTPUT"))
HEADER_OUTPUT = Path(get_env("HEADER_OUTPUT"))
# Auto-scan all SVGs (no config needed)

# --- Font constants ---
UNITS_PER_EM = 1000
ASCENT = 800
DESCENT = 200
EM_CENTER_Y = (ASCENT - DESCENT) // 2
UNICODE_START = 0xE000

def scan_svgs() -> list[Path]:
    """Scan source directory recursively for all SVG files."""
    if not SVG_SOURCE_DIR.exists():
        return []
    return sorted(SVG_SOURCE_DIR.rglob("*.svg"), key=lambda p: p.stem.lower())

# --- Helpers ---
def log(msg: str) -> None:
    print(msg, file=sys.stderr)


def compute_paths_bbox(paths: list[Any]) -> tuple[float, float, float, float] | None:
    """Compute combined bounding box of all paths."""
    xmin, xmax, ymin, ymax = float('inf'), float('-inf'), float('inf'), float('-inf')
    for path in paths:
        if len(path) > 0:
            x0, x1, y0, y1 = path.bbox()
            xmin, xmax = min(xmin, x0), max(xmax, x1)
            ymin, ymax = min(ymin, y0), max(ymax, y1)
    return None if xmin == float('inf') else (xmin, xmax, ymin, ymax)

def svg_to_glyph(svg_path: Path) -> tuple[list[tuple[str, Any]], int] | None:
    """Convert SVG to glyph commands and advance width."""
    try:
        paths = svgpathtools.svg2paths(str(svg_path))[0]
        if not paths:
            return None

        bbox = compute_paths_bbox(paths)
        if not bbox:
            return None

        xmin, xmax, ymin, ymax = bbox
        content_w, content_h = xmax - xmin, ymax - ymin
        if content_w <= 0 or content_h <= 0:
            return None

        scale = UNITS_PER_EM / max(content_w, content_h)
        center_x, center_y = (xmin + xmax) / 2, (ymin + ymax) / 2
        em_center_x = UNITS_PER_EM / 2

        def transform(p: complex) -> tuple[float, float]:
            x = em_center_x + (p.real - center_x) * scale
            y = EM_CENTER_Y + (center_y - p.imag) * scale
            return (max(0, min(x, UNITS_PER_EM)), max(-DESCENT, min(y, ASCENT)))

        commands: list[tuple[str, Any]] = []
        for path in paths:
            if not path:
                continue
            commands.append(('moveTo', transform(path[0].start)))
            for seg in path:
                t = type(seg).__name__
                if t == 'Line':
                    commands.append(('lineTo', transform(seg.end)))
                elif t == 'CubicBezier':
                    c1, c2, end = transform(seg.control1), transform(seg.control2), transform(seg.end)
                    commands.append(('curveTo', (*c1, *c2, *end)))
                elif t == 'QuadraticBezier':
                    ctrl, end = transform(seg.control), transform(seg.end)
                    commands.append(('qCurveTo', (*ctrl, *end)))
                elif t == 'Arc':
                    for cubic in seg.as_cubic_curves():
                        c1, c2, end = transform(cubic.control1), transform(cubic.control2), transform(cubic.end)
                        commands.append(('curveTo', (*c1, *c2, *end)))
            commands.append(('closePath', ()))

        return commands, int(content_w * scale)
    except Exception as e:
        log(f"  warning: {svg_path.name}: {e}")
        return None

# --- Font building ---
def draw_commands(pen: Any, commands: list[tuple[str, Any]]) -> None:
    for cmd, args in commands:
        if cmd == 'moveTo':
            pen.moveTo(args)
        elif cmd == 'lineTo':
            pen.lineTo(args)
        elif cmd == 'curveTo':
            pen.curveTo(args[:2], args[2:4], args[4:6])
        elif cmd == 'qCurveTo':
            pen.qCurveTo(args[:2], args[2:4])
        elif cmd == 'closePath':
            pen.closePath()

def build_ttf(glyphs: dict[str, dict[str, Any]]) -> None:
    glyph_order = ['.notdef'] + list(glyphs.keys())
    cmap = {g['codepoint']: name for name, g in glyphs.items()}

    fb = FontBuilder(UNITS_PER_EM, isTTF=True)
    fb.setupGlyphOrder(glyph_order)
    fb.setupCharacterMap(cmap)

    pen_glyphs: dict[str, Any] = {}

    # .notdef placeholder
    pen = TTGlyphPen(None)
    pen.moveTo((50, 0))
    pen.lineTo((UNITS_PER_EM - 50, 0))
    pen.lineTo((UNITS_PER_EM - 50, ASCENT - 50))
    pen.lineTo((50, ASCENT - 50))
    pen.closePath()
    pen_glyphs['.notdef'] = pen.glyph()

    for name, data in glyphs.items():
        pen = TTGlyphPen(None)
        draw_commands(pen, data['commands'])
        try:
            pen_glyphs[name] = pen.glyph()
        except Exception:
            pen_glyphs[name] = TTGlyphPen(None).glyph()

    fb.setupGlyf(pen_glyphs, validateGlyphFormat=False)

    metrics = {'.notdef': (UNITS_PER_EM, 0)}
    metrics.update({name: (data['advance_width'], 0) for name, data in glyphs.items()})
    fb.setupHorizontalMetrics(metrics)

    fb.setupHead(unitsPerEm=UNITS_PER_EM, glyphDataFormat=1)
    fb.setupHorizontalHeader(ascent=ASCENT, descent=-DESCENT)
    fb.setupMaxp()
    fb.setupOS2(sTypoAscender=ASCENT, sTypoDescender=-DESCENT, sTypoLineGap=0,
                usWinAscent=ASCENT, usWinDescent=DESCENT)
    fb.setupPost()
    fb.setupNameTable({"familyName": "Bitwig Icons", "styleName": "Regular"})

    TTF_OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    fb.save(str(TTF_OUTPUT))

def generate_header(icons: dict[str, int]) -> None:
    max_cp = max(icons.values()) if icons else UNICODE_START

    def to_utf8(cp: int) -> str:
        if cp < 0x80:
            return f'\\x{cp:02X}'
        if cp < 0x800:
            return f'\\x{0xC0 | (cp >> 6):02X}\\x{0x80 | (cp & 0x3F):02X}'
        return f'\\x{0xE0 | (cp >> 12):02X}\\x{0x80 | ((cp >> 6) & 0x3F):02X}\\x{0x80 | (cp & 0x3F):02X}'

    lines = [
        f'// Auto-generated | {len(icons)} icons | U+{UNICODE_START:04X}-U+{max_cp:04X} | {datetime.now().strftime("%Y-%m-%d %H:%M")}',
        '#pragma once',
        '',
        '#include <lvgl.h>',
        '#include "FontLoader.hpp"',
        '',
        'namespace Icon {',
        '    enum Size { S12 = 12, S14 = 14, S18 = 18 };',
        '',
    ]
    for name, cp in sorted(icons.items(), key=lambda x: x[1]):
        lines.append(f'    constexpr const char* {name} = "{to_utf8(cp)}";')
    lines.append('')
    lines.append('    inline void set(lv_obj_t* label, const char* icon, Size size = S14) {')
    lines.append('        lv_font_t* font = (size == S12) ? bitwig_fonts.icons_12')
    lines.append('                        : (size == S18) ? bitwig_fonts.icons_18')
    lines.append('                        : bitwig_fonts.icons_14;')
    lines.append('        lv_obj_set_style_text_font(label, font, 0);')
    lines.append('        lv_label_set_text(label, icon);')
    lines.append('    }')
    lines.append('}  // namespace Icon')

    HEADER_OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    HEADER_OUTPUT.write_text('\n'.join(lines) + '\n', encoding='utf-8')

# --- Main ---
def main() -> int:
    log("● Scanning for SVG icons...")
    svg_files = scan_svgs()

    if not svg_files:
        log(f"✗ No SVG files found in {SVG_SOURCE_DIR}")
        return 1

    log(f"  Found {len(svg_files)} SVG files")

    log("● Converting to font glyphs...")
    glyphs: dict[str, dict[str, Any]] = {}
    icons: dict[str, int] = {}
    codepoint = UNICODE_START

    for svg_path in svg_files:
        result = svg_to_glyph(svg_path)
        if not result:
            continue

        commands, width = result
        glyph_name = f"uni{codepoint:04X}"
        cpp_name = re.sub(r'[^a-zA-Z0-9]+', '_', svg_path.stem).strip('_').upper()

        glyphs[glyph_name] = {'commands': commands, 'codepoint': codepoint, 'advance_width': width}
        icons[cpp_name] = codepoint
        log(f"  ✓ {svg_path.stem} → {cpp_name}")
        codepoint += 1

    if not glyphs:
        log("✗ No valid icons converted")
        return 1

    log(f"● Building TTF ({len(glyphs)} glyphs)...")
    build_ttf(glyphs)
    log(f"  ✓ {TTF_OUTPUT}")

    log("● Generating C++ header...")
    generate_header(icons)
    log(f"  ✓ {HEADER_OUTPUT}")

    print(f"0x{UNICODE_START:04X}-0x{codepoint - 1:04X}")
    return 0

if __name__ == "__main__":
    sys.exit(main())
