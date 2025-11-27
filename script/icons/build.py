#!/usr/bin/env python3
"""
Icon Font Builder - SVG → TTF + C++ header with incremental builds
"""

import sys
import os

# Force UTF-8 output on Windows
if sys.platform == 'win32':
    sys.stdout.reconfigure(encoding='utf-8')
    sys.stderr.reconfigure(encoding='utf-8')
    os.environ['PYTHONIOENCODING'] = 'utf-8'

import subprocess
import hashlib
import json
import re
from pathlib import Path
from datetime import datetime
from xml.etree import ElementTree as ET

# Find project root
SCRIPT_DIR = Path(__file__).parent
PROJECT_ROOT = SCRIPT_DIR.parent.parent

sys.path.insert(0, str(SCRIPT_DIR))
from config import *

# === Colors & Logging ===

class C:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    BOLD = '\033[1m'
    DIM = '\033[2m'
    NC = '\033[0m'

def log(msg):      print(f"{C.BLUE}●{C.NC} {msg}")
def success(msg):  print(f"  {C.GREEN}✓{C.NC} {msg}")
def warn(msg):     print(f"  {C.YELLOW}⚠{C.NC} {msg}")
def error(msg):    print(f"{C.RED}✗{C.NC} {msg}", file=sys.stderr); sys.exit(1)
def added(msg):    print(f"  {C.GREEN}+{C.NC} {C.DIM}{msg}{C.NC}")
def modified(msg): print(f"  {C.YELLOW}~{C.NC} {C.DIM}{msg}{C.NC}")
def removed(msg):  print(f"  {C.RED}-{C.NC} {C.DIM}{msg}{C.NC}")
def skip(msg):     print(f"  {C.DIM}  {msg}{C.NC}")


# === Hash & Cache ===

def file_hash(path: Path) -> str:
    return hashlib.md5(path.read_bytes()).hexdigest()[:12]

def load_hashes(cache_dir: Path) -> dict:
    hash_file = cache_dir / "hashes.json"
    if hash_file.exists():
        return json.loads(hash_file.read_text())
    return {}

def save_hashes(cache_dir: Path, hashes: dict):
    (cache_dir / "hashes.json").write_text(json.dumps(hashes, indent=2))


# === SVG Cleaning ===

def inkscape_run(src: Path, dst: Path, actions: list[str] = None, fit: bool = False) -> bool:
    cmd = [INKSCAPE, str(src), "--export-type=svg", "--export-plain-svg", f"--export-filename={dst}"]
    if fit:
        cmd.append("--export-area-drawing")
    if actions:
        cmd.append(f"--actions={';'.join(actions)}")
    subprocess.run(cmd, capture_output=True)
    return dst.exists()

def xml_cleanup(src: Path, dst: Path):
    ET.register_namespace('', 'http://www.w3.org/2000/svg')
    tree = ET.parse(src)
    root = tree.getroot()

    def tag_name(e): return e.tag.split('}')[-1] if '}' in e.tag else e.tag
    def remove(e):
        for p in root.iter():
            if e in list(p): p.remove(e); return

    for elem in list(root.iter()):
        tag = tag_name(elem)
        style, fill = elem.get('style', ''), elem.get('fill', '')
        if ('fill:none' in style or fill == 'none') and 'stroke:' not in style:
            remove(elem)
        elif tag in ('metadata',) or any(x in elem.tag for x in ['sodipodi', 'inkscape', 'rdf']):
            remove(elem)
        elif tag in ('defs', 'g') and not list(elem):
            remove(elem)

    for elem in root.iter():
        if tag_name(elem) == 'path':
            for attr in ['style', 'fill']:
                if attr in elem.attrib: del elem.attrib[attr]
            elem.set('fill-rule', 'nonzero')

    for attr in list(root.attrib.keys()):
        if any(x in attr for x in ['sodipodi', 'inkscape', 'rdf', 'dc', 'cc', 'style']):
            del root.attrib[attr]

    tree.write(dst, encoding='unicode', xml_declaration=True)

def add_padding(src: Path, dst: Path):
    tree = ET.parse(src)
    root = tree.getroot()
    vb = root.get('viewBox', '').split()
    if len(vb) == 4:
        vx, vy, vw, vh = map(float, vb)
    else:
        vx, vy = 0, 0
        vw = float(root.get('width', '100').replace('px', ''))
        vh = float(root.get('height', '100').replace('px', ''))
    pad = vh * PADDING_PERCENT
    vx, vy, vw, vh = vx - pad, vy - pad, vw + 2*pad, vh + 2*pad
    root.set('viewBox', f'{vx:.4f} {vy:.4f} {vw:.4f} {vh:.4f}')
    tree.write(dst, encoding='unicode', xml_declaration=True)

def clean_svg(src: Path, dst: Path, temp_dir: Path) -> bool:
    t1, t2, t3 = temp_dir / "1.svg", temp_dir / "2.svg", temp_dir / "3.svg"
    if not inkscape_run(src, t1, ["select-all", "object-to-path", "select-all", "object-stroke-to-path", "export-plain-svg", "export-do"]):
        return False
    try:
        xml_cleanup(t1, t2)
    except:
        return False
    if not inkscape_run(t2, t3, fit=True):
        return False
    try:
        add_padding(t3, dst)
    except:
        return False
    for f in [t1, t2, t3]:
        f.unlink(missing_ok=True)
    return True


# === Font Generation ===

FF_SCRIPT = '''
import fontforge, os, sys
svg_dir, output = sys.argv[1], sys.argv[2]
font = fontforge.font()
font.fontname, font.familyname = "{name}", "{family}"
font.fullname = "{family} Regular"
font.em, font.ascent, font.descent = {em}, {ascent}, {descent}
for i, f in enumerate(sorted(f for f in os.listdir(svg_dir) if f.endswith('.svg'))):
    cp = {start} + i
    g = font.createChar(cp)
    g.importOutlines(os.path.join(svg_dir, f))
    g.removeOverlap()
    g.correctDirection()
    bbox = g.boundingBox()
    if bbox:
        w = bbox[2] - bbox[0]
        g.transform([1, 0, 0, 1, {margin} - bbox[0], 0])
        g.width = int(w + 2 * {margin})
    print(f"GLYPH|{{cp}}|{{os.path.splitext(f)[0]}}")
font.generate(output)
'''

def generate_font(svg_dir: Path, ttf_path: Path) -> list[tuple[str, int]]:
    script = FF_SCRIPT.format(
        name=FONT_NAME, family=FONT_FAMILY, em=UNITS_PER_EM,
        ascent=ASCENT, descent=DESCENT, start=UNICODE_START, margin=GLYPH_MARGIN
    )
    script_path = svg_dir / "_gen.py"
    script_path.write_text(script)
    result = subprocess.run([FONTFORGE, "-script", str(script_path), str(svg_dir), str(ttf_path)],
                           capture_output=True, text=True)
    script_path.unlink()
    return [(name, int(cp)) for line in result.stdout.split('\n')
            if line.startswith('GLYPH|') for _, cp, name in [line.split('|')]]

def generate_header(glyphs: list[tuple[str, int]], path: Path, sizes: list[int]):
    def utf8(cp):
        return f'\\x{0xE0|(cp>>12):02X}\\x{0x80|((cp>>6)&0x3F):02X}\\x{0x80|(cp&0x3F):02X}'

    # Generate Size enum
    size_enum = ', '.join(f'S{s} = {s}' for s in sizes)
    default_size = sizes[len(sizes)//2]  # Middle size as default

    # Generate font selection (uses bitwig_fonts from FontLoader.hpp)
    font_cases = []
    for i, s in enumerate(sizes):
        if i == 0:
            font_cases.append(f'(size == S{s}) ? bitwig_fonts.icons_{s}')
        elif i == len(sizes) - 1:
            font_cases.append(f'bitwig_fonts.icons_{s}')
        else:
            font_cases.append(f'(size == S{s}) ? bitwig_fonts.icons_{s}')

    lines = [
        f'// Auto-generated | {len(glyphs)} icons | {datetime.now():%Y-%m-%d}',
        '#pragma once', '#include <lvgl.h>', '#include "FontLoader.hpp"', '',
        'namespace Icon {', f'    enum Size {{ {size_enum} }};', ''
    ]
    for name, cp in glyphs:
        cname = re.sub(r'[^a-zA-Z0-9]+', '_', name).strip('_').upper()
        lines.append(f'    constexpr const char* {cname} = "{utf8(cp)}";')

    # Build font selection chain
    font_select = '\n                        : '.join(font_cases)

    lines += ['',
        f'    inline void set(lv_obj_t* label, const char* icon, Size size = S{default_size}) {{',
        f'        lv_font_t* font = {font_select};',
        '        lv_obj_set_style_text_font(label, font, 0);',
        '        lv_label_set_text(label, icon);',
        '    }',
        '}  // namespace Icon'
    ]
    path.write_text('\n'.join(lines), encoding='utf-8')


# === LVGL Font Generation ===

def generate_lvgl_fonts(ttf_path: Path, out_dir: Path, glyphs: list[tuple[str, int]], sizes: list[int], bpp: int) -> bool:
    """Generate LVGL binary fonts using npx lv_font_conv."""
    import shutil
    import tempfile

    # Find npx (on Windows it's npx.cmd)
    npx = shutil.which('npx')
    if not npx:
        warn("npx not found, skipping LVGL generation")
        warn("Install Node.js to get npx")
        return False

    # Unicode range from glyphs
    if not glyphs:
        return False
    min_cp = min(cp for _, cp in glyphs)
    max_cp = max(cp for _, cp in glyphs)
    char_range = f"0x{min_cp:04X}-0x{max_cp:04X}"

    data_dir = out_dir / "data"
    data_dir.mkdir(exist_ok=True)

    for size in sizes:
        arr_name = f"{FONT_NAME}_{size}_bin"
        out_name = f"{FONT_NAME}_{size}"

        # Generate binary
        with tempfile.NamedTemporaryFile(delete=False, suffix='.bin') as tmp:
            bin_file = Path(tmp.name)

        cmd = [
            npx, 'lv_font_conv',
            '--font', str(ttf_path),
            '--size', str(size),
            '--format', 'bin',
            '--bpp', str(bpp),
            '--range', char_range,
            '--no-kerning',
            '-o', str(bin_file)
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0 or not bin_file.exists():
            warn(f"lv_font_conv failed for size {size}")
            continue

        bin_size = bin_file.stat().st_size
        bin_data = bin_file.read_bytes()
        bin_file.unlink()

        # Generate C file
        cpp_file = data_dir / f"{out_name}.c.inc"
        hex_data = ', '.join(f'0x{b:02x}' for b in bin_data)
        # Format in rows of 16 bytes
        hex_lines = [hex_data[i:i+16*6-2] for i in range(0, len(hex_data), 16*6)]

        cpp_content = f'''// Auto-generated | {FONT_NAME} | {size}px | {bpp}bpp | {datetime.now():%Y-%m-%d %H:%M}

const uint8_t {arr_name}[] PROGMEM = {{
    {(','+chr(10)+'    ').join(hex_lines)}
}};
const uint32_t {arr_name}_len = {bin_size};
'''
        cpp_file.write_text(cpp_content, encoding='utf-8')

        # Generate header
        hpp_file = data_dir / f"{out_name}.hpp"
        hpp_content = f'''// Auto-generated | {FONT_NAME} | {size}px | {bpp}bpp | {datetime.now():%Y-%m-%d %H:%M}
#pragma once
#include <Arduino.h>
extern const uint8_t {arr_name}[] PROGMEM;
extern const uint32_t {arr_name}_len;
'''
        hpp_file.write_text(hpp_content, encoding='utf-8')
        success(f"{out_name} ({bin_size} bytes)")

    return True


# === Main ===

def main():
    print(f"\n{C.BOLD}{C.BLUE}═══ Icon Font Builder ═══{C.NC}\n")

    # Paths
    src_dir = PROJECT_ROOT / SVG_SOURCE_DIR
    ttf_dir = PROJECT_ROOT / TTF_OUTPUT_DIR
    header_dir = PROJECT_ROOT / HEADER_OUTPUT_DIR
    cache_dir = PROJECT_ROOT / CACHE_DIR
    svg_cache = cache_dir / "svg"
    temp_dir = cache_dir / "temp"

    if not src_dir.exists():
        error(f"Source not found: {SVG_SOURCE_DIR}")

    print(f"{C.DIM}Source: {src_dir}")
    print(f"TTF:    {ttf_dir}")
    print(f"Header: {header_dir}")
    print(f"Cache:  {cache_dir}{C.NC}\n")

    # Setup dirs
    ttf_dir.mkdir(parents=True, exist_ok=True)
    header_dir.mkdir(parents=True, exist_ok=True)
    svg_cache.mkdir(parents=True, exist_ok=True)
    temp_dir.mkdir(parents=True, exist_ok=True)

    # Load hashes
    old_hashes = load_hashes(cache_dir)
    new_hashes = {}

    # Find source SVGs
    src_svgs = {f.name: f for f in sorted(src_dir.glob("*.svg"))}

    # === Step 1: Process SVGs ===
    log(f"Processing SVGs ({len(src_svgs)} files)")

    stats = {'added': 0, 'modified': 0, 'unchanged': 0, 'removed': 0}
    need_rebuild = False

    for name, src_path in src_svgs.items():
        h = file_hash(src_path)
        new_hashes[name] = h
        cached_svg = svg_cache / name

        if name not in old_hashes:
            # New file
            if clean_svg(src_path, cached_svg, temp_dir):
                added(name)
                stats['added'] += 1
                need_rebuild = True
            else:
                warn(f"Failed: {name}")
        elif old_hashes[name] != h:
            # Modified file
            if clean_svg(src_path, cached_svg, temp_dir):
                modified(name)
                stats['modified'] += 1
                need_rebuild = True
            else:
                warn(f"Failed: {name}")
        else:
            # Unchanged
            if not cached_svg.exists():
                # Cache missing, regenerate
                if clean_svg(src_path, cached_svg, temp_dir):
                    modified(f"{name} (cache rebuilt)")
                    stats['modified'] += 1
                    need_rebuild = True
            else:
                skip(name)
                stats['unchanged'] += 1

    # Remove deleted files
    for name in old_hashes:
        if name not in src_svgs:
            cached = svg_cache / name
            if cached.exists():
                cached.unlink()
            removed(name)
            stats['removed'] += 1
            need_rebuild = True

    # Save hashes
    save_hashes(cache_dir, new_hashes)

    # Summary
    parts = []
    if stats['added']: parts.append(f"{C.GREEN}+{stats['added']}{C.NC}")
    if stats['modified']: parts.append(f"{C.YELLOW}~{stats['modified']}{C.NC}")
    if stats['removed']: parts.append(f"{C.RED}-{stats['removed']}{C.NC}")
    if stats['unchanged']: parts.append(f"{C.DIM}={stats['unchanged']}{C.NC}")
    success(f"SVGs: {' '.join(parts)}")

    # === Step 2: Generate Font ===
    ttf_path = ttf_dir / f"{FONT_NAME}.ttf"
    header_path = header_dir / "Icon.hpp"

    # Check if LVGL fonts need rebuild
    lvgl_files_exist = all(
        (ttf_dir / "data" / f"{FONT_NAME}_{s}.c.inc").exists()
        for s in LVGL_FONT_SIZES
    )

    if not need_rebuild and ttf_path.exists() and header_path.exists() and lvgl_files_exist:
        print(f"\n{C.GREEN}✓{C.NC} {C.DIM}No changes, font up to date{C.NC}")
    else:
        # Generate TTF
        log("Generating TTF")
        glyphs = generate_font(svg_cache, ttf_path)
        if not glyphs:
            error("FontForge failed")
        success(f"{ttf_path.name} ({len(glyphs)} glyphs)")

        # Generate Icon.hpp
        generate_header(glyphs, header_path, LVGL_FONT_SIZES)
        success(header_path.name)

        # Generate LVGL fonts
        log(f"Generating LVGL fonts ({', '.join(map(str, LVGL_FONT_SIZES))}px)")
        generate_lvgl_fonts(ttf_path, ttf_dir, glyphs, LVGL_FONT_SIZES, LVGL_BPP)

    # Cleanup temp
    for f in temp_dir.iterdir():
        f.unlink()

    print(f"\n{C.GREEN}✓ Done!{C.NC}\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
