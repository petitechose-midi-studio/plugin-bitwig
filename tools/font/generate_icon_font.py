#!/usr/bin/env python3
"""
Bitwig Icon Font Generator

Converts SVG icons to TTF font + C++ header for LVGL.
Called by: generate_lvgl_binary_font.sh
Output: TTF file, C++ header, unicode range on stdout
"""

from __future__ import annotations

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

# =============================================================================
# CONFIGURATION
# =============================================================================

PROJECT_ROOT = Path(__file__).parent.parent.parent.absolute()
SVG_SOURCE_DIR = Path(r"C:\Program Files\Bitwig Studio 6.0 Beta 6\resources\icons")
TTF_OUTPUT = PROJECT_ROOT / "tools" / "font" / "bitwig_icons.ttf"
HEADER_OUTPUT = PROJECT_ROOT / "src" / "ui" / "font" / "bitwig_icons.hpp"

UNITS_PER_EM = 1000
ASCENT = 800
DESCENT = 200
EM_CENTER_Y = (ASCENT - DESCENT) // 2  # 300
UNICODE_START = 0xE000

SELECTED_ICONS = [
    # Transport
    "transport_play", "transport_stop", "transport_record",
    # UI (LVGLSymbol aliases)
    "arrow_left",    # BACK
    "multi_layer",   # LAYER
    "drum_pads",     # DRUM_PAD
    "group_track",   # FOLDER
]

# =============================================================================
# SVG TO GLYPH CONVERSION
# =============================================================================


def log(msg: str) -> None:
    print(msg, file=sys.stderr)


def find_svg(name: str) -> Path | None:
    """Find SVG file by name in source directory."""
    direct = SVG_SOURCE_DIR / f"{name}.svg"
    if direct.exists():
        return direct
    for f in SVG_SOURCE_DIR.rglob("*.svg"):
        if f.stem.lower() == name.lower():
            return f
    return None


def compute_paths_bbox(paths: list[Any]) -> tuple[float, float, float, float] | None:
    """Compute combined bounding box of all paths."""
    xmin, xmax, ymin, ymax = float('inf'), float('-inf'), float('inf'), float('-inf')
    for path in paths:
        if len(path) > 0:
            x0, x1, y0, y1 = path.bbox()
            xmin, xmax = min(xmin, x0), max(xmax, x1)
            ymin, ymax = min(ymin, y0), max(ymax, y1)
    if xmin == float('inf'):
        return None
    return xmin, xmax, ymin, ymax


def svg_to_glyph(svg_path: Path) -> tuple[list[tuple[str, Any]], int] | None:
    """Convert SVG to glyph commands and advance width. Returns None on failure."""
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

        # Scale largest dimension to fill em, center content
        scale = UNITS_PER_EM / max(content_w, content_h)
        center_x, center_y = (xmin + xmax) / 2, (ymin + ymax) / 2
        em_center_x = UNITS_PER_EM / 2

        def transform(p: complex) -> tuple[float, float]:
            x = em_center_x + (p.real - center_x) * scale
            y = EM_CENTER_Y + (center_y - p.imag) * scale
            return (
                max(0, min(x, UNITS_PER_EM)),
                max(-DESCENT, min(y, ASCENT))
            )

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


# =============================================================================
# FONT BUILDING
# =============================================================================


def draw_commands(pen: Any, commands: list[tuple[str, Any]]) -> None:
    """Execute drawing commands on a pen."""
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
    """Build and save TTF font file."""
    glyph_order = ['.notdef'] + list(glyphs.keys())
    cmap = {g['codepoint']: name for name, g in glyphs.items()}

    fb = FontBuilder(UNITS_PER_EM, isTTF=True)
    fb.setupGlyphOrder(glyph_order)
    fb.setupCharacterMap(cmap)

    # Build glyph outlines
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

    # Metrics: actual width per glyph
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
    """Generate C++ header with icon constants."""
    max_cp = max(icons.values()) if icons else UNICODE_START

    def to_utf8(cp: int) -> str:
        if cp < 0x80:
            return f'\\x{cp:02X}'
        if cp < 0x800:
            return f'\\x{0xC0 | (cp >> 6):02X}\\x{0x80 | (cp & 0x3F):02X}'
        return f'\\x{0xE0 | (cp >> 12):02X}\\x{0x80 | ((cp >> 6) & 0x3F):02X}\\x{0x80 | (cp & 0x3F):02X}'

    lines = [
        '#pragma once',
        '/**',
        f' * @file {HEADER_OUTPUT.name}',
        f' * @brief Bitwig icon font character mappings for LVGL',
        f' * Auto-generated on {datetime.now().strftime("%Y-%m-%d %H:%M")}',
        f' * Icons: {len(icons)} | Range: U+{UNICODE_START:04X}-U+{max_cp:04X}',
        ' */',
        '',
        'namespace BitwigIcon {',
    ]
    for name, cp in sorted(icons.items(), key=lambda x: x[1]):
        lines.append(f'    constexpr const char* {name} = "{to_utf8(cp)}";')
    lines.append('}  // namespace BitwigIcon')

    HEADER_OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    HEADER_OUTPUT.write_text('\n'.join(lines) + '\n', encoding='utf-8')


# =============================================================================
# MAIN
# =============================================================================


def main() -> int:
    log("● Generating Bitwig icon font...")

    glyphs: dict[str, dict[str, Any]] = {}
    icons: dict[str, int] = {}
    codepoint = UNICODE_START
    skipped = 0

    for icon_name in SELECTED_ICONS:
        svg_path = find_svg(icon_name)
        if not svg_path:
            log(f"  ✗ {icon_name} (not found)")
            skipped += 1
            continue

        result = svg_to_glyph(svg_path)
        if not result:
            log(f"  ✗ {icon_name} (no paths)")
            skipped += 1
            continue

        commands, width = result
        glyph_name = f"uni{codepoint:04X}"
        cpp_name = re.sub(r'[^a-zA-Z0-9]+', '_', icon_name).strip('_').upper()

        glyphs[glyph_name] = {'commands': commands, 'codepoint': codepoint, 'advance_width': width}
        icons[cpp_name] = codepoint
        codepoint += 1

    if not glyphs:
        log("✗ No icons imported")
        return 1

    log(f"  ✓ {len(glyphs)} icons" + (f", {skipped} skipped" if skipped else ""))

    log("● Building TTF...")
    build_ttf(glyphs)
    log(f"  ✓ {TTF_OUTPUT.relative_to(PROJECT_ROOT)}")

    log("● Generating header...")
    generate_header(icons)
    log(f"  ✓ {HEADER_OUTPUT.relative_to(PROJECT_ROOT)}")

    end_cp = codepoint - 1
    print(f"0x{UNICODE_START:04X}-0x{end_cp:04X}")
    log(f"● Done ({len(glyphs)} icons)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
