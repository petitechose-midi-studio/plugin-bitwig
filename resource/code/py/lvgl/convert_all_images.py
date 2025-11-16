#!/usr/bin/env python3
"""
Script pour convertir tous les PNG du dossier bitwig/resource/img en format LVGL
Usage: python convert_all_images.py
"""

import sys
from convert_image import convert_image, find_project_root

def convert_all_images():
    """Convertit tous les fichiers PNG du dossier img"""

    # Trouver le dossier des images
    project_root = find_project_root()
    img_dir = project_root / "plugin" / "bitwig" / "resource" / "img"

    if not img_dir.exists():
        print(f"[ERROR] Dossier introuvable: {img_dir}")
        sys.exit(1)

    # Trouver tous les PNG
    png_files = list(img_dir.glob("*.png"))

    if not png_files:
        print(f"[WARNING] Aucun fichier PNG trouve dans {img_dir}")
        return

    print(f"Dossier: {img_dir}")
    print(f"Trouve {len(png_files)} fichier(s) PNG\n")

    # Convertir chaque fichier
    success_count = 0
    error_count = 0

    for png_file in png_files:
        print(f"{'='*60}")
        try:
            # Générer le nom de sortie (remplace .png par .c)
            output_path = png_file.with_suffix('.c')

            convert_image(
                input_path=str(png_file),
                output_format="c",
                color_format="ARGB8565",
                compress="LZ4",
                output_path=str(output_path)
            )
            success_count += 1

        except Exception as e:
            print(f"[ERROR] Erreur avec {png_file.name}: {e}")
            error_count += 1

        print()  # Ligne vide entre les conversions

    # Résumé
    print(f"{'='*60}")
    print(f"[SUCCESS] Conversions reussies: {success_count}")
    if error_count > 0:
        print(f"[ERROR] Erreurs: {error_count}")
    print(f"\nTermine!")

if __name__ == "__main__":
    try:
        convert_all_images()
    except Exception as e:
        print(f"\n[FATAL ERROR] {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
