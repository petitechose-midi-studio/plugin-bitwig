#!/usr/bin/env python3
"""
Script générique de conversion d'images PNG vers LVGL
Usage: python convert_image.py <fichier.png> [options]
"""

import sys
import os
import argparse
from pathlib import Path

# Détecter la racine du projet en cherchant platformio.ini
def find_project_root():
    current = Path(__file__).resolve().parent
    while current != current.parent:
        if (current / "platformio.ini").exists():
            return current
        current = current.parent
    raise FileNotFoundError("Impossible de trouver la racine du projet (platformio.ini)")

project_root = find_project_root()
lvgl_scripts_path = project_root / ".pio" / "libdeps" / "debug" / "lvgl" / "scripts"
sys.path.insert(0, str(lvgl_scripts_path))

from LVGLImage import LVGLImage, ColorFormat, CompressMethod  # type: ignore[import-not-found]

def convert_image(input_path: str, output_format: str = "c", color_format: str = "ARGB8565",
                  compress: str = "LZ4", output_path: str | None = None) -> LVGLImage:
    """
    Convertit une image PNG en format LVGL

    Args:
        input_path: Chemin du PNG source
        output_format: "bin" ou "c"
        color_format: Format de couleur (RGB565, ARGB8888, etc.)
        compress: Méthode de compression (NONE, LZ4, RLE)
        output_path: Chemin de sortie (optionnel)
    """

    if not os.path.exists(input_path):
        raise FileNotFoundError(f"Fichier introuvable: {input_path}")

    # Déterminer le chemin de sortie
    if output_path is None:
        base = os.path.splitext(input_path)[0]
        ext = ".bin" if output_format == "bin" else ".c"
        output_path = base + ext

    print(f"Conversion d'image LVGL")
    print(f"   Entree:  {input_path}")
    print(f"   Sortie:  {output_path}")

    # Mapper les formats de couleur
    cf_map = {
        "RGB565": ColorFormat.RGB565,
        "ARGB8888": ColorFormat.ARGB8888,
        "ARGB8565": ColorFormat.ARGB8565,
        "RGB565A8": ColorFormat.RGB565A8,
        "L8": ColorFormat.L8,
        "A8": ColorFormat.A8,
    }

    compress_map = {
        "NONE": CompressMethod.NONE,
        "LZ4": CompressMethod.LZ4,
        "RLE": CompressMethod.RLE,
    }

    cf = cf_map.get(color_format.upper())
    if cf is None:
        raise ValueError(f"Format de couleur invalide: {color_format}")

    compress_method = compress_map.get(compress.upper())
    if compress_method is None:
        raise ValueError(f"Méthode de compression invalide: {compress}")

    # Créer l'image LVGL
    img = LVGLImage().from_png(input_path, cf=cf, background=0x000000)

    print(f"   Format:  {img.cf.name}")
    print(f"   Taille:  {img.w}x{img.h} pixels")
    print(f"   Données: {img.data_len:,} bytes (non compressé)")

    # Optimiser l'alignement
    img.adjust_stride(align=1)

    # Générer le fichier de sortie
    var_name = os.path.splitext(os.path.basename(input_path))[0].replace("-", "_").replace(".", "_")

    if output_format == "bin":
        img.to_bin(output_path, compress=compress_method)
        file_size = os.path.getsize(output_path)
        ratio = (1 - file_size / img.data_len) * 100 if img.data_len > 0 else 0
        print(f"   [OK] Fichier BIN: {file_size:,} bytes")
        if compress_method != CompressMethod.NONE:
            print(f"   Compression: {ratio:.1f}%")
    else:  # format C
        img.to_c_array(output_path, compress=compress_method, outputname=var_name)

        # Post-process: Add PROGMEM attribute for Teensy (store in FLASH)
        with open(output_path, 'r') as f:
            content = f.read()

        # Add pgmspace.h include for PROGMEM (after LVGL includes)
        lvgl_endif = "#endif"
        insert_pos = content.find(lvgl_endif) + len(lvgl_endif)
        if insert_pos > len(lvgl_endif):
            content = (content[:insert_pos] +
                      "\n\n#ifdef ARDUINO\n#include <avr/pgmspace.h>  // For PROGMEM definition\n#endif" +
                      content[insert_pos:])

        # Replace the empty attribute definition with PROGMEM
        macro_name = f"LV_ATTRIBUTE_{var_name.upper()}"
        content = content.replace(
            f"#ifndef {macro_name}\n#define {macro_name}\n#endif",
            f"#ifndef {macro_name}\n#define {macro_name} PROGMEM\n#endif"
        )

        with open(output_path, 'w') as f:
            f.write(content)

        file_size = os.path.getsize(output_path)
        print(f"   [OK] Fichier C: {file_size:,} bytes (PROGMEM/FLASH)")
        print(f"   Variable: {var_name}")

    print(f"\nUtilisation:")
    if output_format == "bin":
        print(f'   lv_obj_t* img = lv_image_create(parent);')
        print(f'   lv_image_set_src(img, "S:{output_path.replace(os.sep, "/")}");')
    else:
        print(f'   LV_IMG_DECLARE({var_name});')
        print(f'   lv_image_set_src(img, &{var_name});')

    return img

def main():
    parser = argparse.ArgumentParser(
        description="Convertit une image PNG en format LVGL pour Teensy",
        epilog="Exemple: python convert_image.py logo.png --format RGB565 --compress LZ4"
    )

    parser.add_argument("input", help="Fichier PNG à convertir")
    parser.add_argument("-o", "--output", help="Fichier de sortie (optionnel)")
    parser.add_argument("-f", "--format",
                       choices=["RGB565", "ARGB8888", "ARGB8565", "RGB565A8", "L8", "A8"],
                       default="ARGB8565",
                       help="Format de couleur (défaut: ARGB8565)")
    parser.add_argument("-c", "--compress",
                       choices=["NONE", "LZ4", "RLE"],
                       default="LZ4",
                       help="Compression (défaut: LZ4)")
    parser.add_argument("-t", "--type",
                       choices=["bin", "c"],
                       default="c",
                       help="Type de sortie (défaut: c)")

    args = parser.parse_args()

    try:
        convert_image(
            args.input,
            output_format=args.type,
            color_format=args.format,
            compress=args.compress,
            output_path=args.output
        )
        print("\n[SUCCESS] Conversion reussie!")
    except Exception as e:
        print(f"\n[ERROR] Erreur: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)

if __name__ == "__main__":
    main()
