#!/usr/bin/env python3
"""Quick test script for SVG processing"""

import sys
import subprocess
from pathlib import Path
from xml.etree import ElementTree as ET

sys.path.insert(0, str(Path(__file__).parent))
from config import INKSCAPE, PADDING_PERCENT

PROJECT = Path(__file__).parent.parent.parent
SRC_DIR = PROJECT / "asset/icon"
OUT_DIR = PROJECT / ".cache/icons/test"
OUT_DIR.mkdir(parents=True, exist_ok=True)

# Test avec ces icônes seulement
TEST_ICONS = ["mute.svg", "solo.svg", "arrow_left.svg"]

def inkscape_run(src, dst, actions=None, fit=False):
    cmd = [INKSCAPE, str(src), "--export-type=svg", "--export-plain-svg", f"--export-filename={dst}"]
    if fit:
        cmd.append("--export-area-drawing")
    if actions:
        cmd.append(f"--actions={';'.join(actions)}")
    subprocess.run(cmd, capture_output=True)
    return dst.exists()

def show_svg(path, label=""):
    tree = ET.parse(path)
    root = tree.getroot()
    vb = root.get('viewBox', 'N/A')
    # Check for transform groups
    ns = '{http://www.w3.org/2000/svg}'
    groups = root.findall(f'.//{ns}g[@transform]')
    transforms = [g.get('transform') for g in groups]
    # Get first path coords
    paths = root.findall(f'.//{ns}path')
    first_path = paths[0].get('d', '')[:50] if paths else 'N/A'

    print(f"  {label}: viewBox={vb}")
    if transforms:
        print(f"       transforms: {transforms}")
    print(f"       path start: {first_path}...")

def process_icon(name):
    src = SRC_DIR / name
    if not src.exists():
        print(f"❌ {name} not found")
        return

    print(f"\n{'='*60}")
    print(f"Processing: {name}")
    print(f"{'='*60}")

    t1 = OUT_DIR / f"{name}.1_stroke.svg"
    t2 = OUT_DIR / f"{name}.2_fit.svg"
    t3 = OUT_DIR / f"{name}.3_square.svg"
    t4 = OUT_DIR / f"{name}.4_baked.svg"

    # Check original viewBox
    orig_tree = ET.parse(src)
    orig_root = orig_tree.getroot()
    orig_vb = orig_root.get('viewBox', '').split()
    has_orig_vb = len(orig_vb) == 4

    # Show source
    show_svg(src, "SOURCE")

    # Step 1: stroke to path (always needed)
    inkscape_run(src, t1, ["select-all", "object-to-path", "select-all", "object-stroke-to-path", "export-plain-svg", "export-do"])
    show_svg(t1, "STROKE→PATH")

    if has_orig_vb:
        # Has viewBox → keep it as-is (author defined proportions and centering)
        ox, oy, ow, oh = map(float, orig_vb)
        print(f"  HAS VIEWBOX: keeping ({ox},{oy},{ow},{oh})")
        import shutil
        shutil.copy(t1, t3)
    else:
        # No viewBox → fit to content, then add padding and center
        inkscape_run(t1, t2, fit=True)
        show_svg(t2, "FIT")

        tree = ET.parse(t2)
        root = tree.getroot()
        vb = root.get('viewBox', '').split()
        vx, vy, vw, vh = map(float, vb) if len(vb) == 4 else (0, 0, 100, 100)

        pad = vh * PADDING_PERCENT
        size = max(vw, vh) + 2 * pad
        print(f"  NO VIEWBOX: bbox=({vx},{vy},{vw},{vh}) pad={pad:.2f} size={size:.2f}")

        # Center content by adjusting viewBox origin
        new_x = vx - (size - vw) / 2
        new_y = vy - (size - vh) / 2
        root.set('viewBox', f'{new_x:.4f} {new_y:.4f} {size:.4f} {size:.4f}')
        root.set('width', f'{size:.4f}')
        root.set('height', f'{size:.4f}')
        tree.write(t3, encoding='unicode', xml_declaration=True)

    show_svg(t3, "FINAL")

if __name__ == "__main__":
    for icon in TEST_ICONS:
        process_icon(icon)
    print(f"\n✓ Output in: {OUT_DIR}")
