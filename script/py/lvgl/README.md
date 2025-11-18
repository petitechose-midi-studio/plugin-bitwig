# Scripts de Conversion d'Images LVGL pour Bitwig Plugin

Ce dossier contient les scripts Python pour convertir des images PNG en format LVGL binaire optimisé pour Teensy 4.1.

## Prérequis

```bash
pip install pypng lz4
```

## Scripts Disponibles

### 1. `convert_image.py` - Script Générique

Convertit n'importe quelle image PNG en format LVGL.

**Usage:**
```bash
python convert_image.py <fichier.png> [options]
```

**Options:**
- `-f, --format`: Format de couleur
  - `ARGB8565` (défaut) - 24-bit avec alpha (pour transparence)
  - `RGB565` - 16-bit sans alpha
  - `ARGB8888` - 32-bit alpha complet
  - `L8` - 8-bit grayscale

- `-c, --compress`: Compression
  - `LZ4` (défaut) - Recommandé
  - `RLE` - Alternative
  - `NONE` - Sans compression

- `-t, --type`: Type de sortie
  - `c` (défaut) - Fichier C array (intégré au firmware)
  - `bin` - Fichier binaire compact (pour filesystem)

- `-o, --output`: Chemin de sortie personnalisé

**Exemples:**
```bash
# Conversion par défaut (ARGB8565 + LZ4 + C array)
python convert_image.py icon.png

# Image sans transparence
python convert_image.py photo.png -f RGB565

# Format binaire pour filesystem
python convert_image.py image.png -t bin
```

### 2. `convert_all_images.py` - Conversion en Masse

Convertit automatiquement tous les PNG du dossier `plugin/bitwig/resource/img` avec les paramètres optimaux.

**Usage:**
```bash
python convert_all_images.py
# Ou sur Windows:
convert_all.bat
```

Génère un fichier `.c` pour chaque PNG avec:
- Format: ARGB8565 (transparence préservée)
- Compression: LZ4
- Type: C array (intégré au firmware)

## Détection Automatique des Chemins

Les scripts utilisent une détection automatique de la racine du projet en cherchant `platformio.ini`. Vous pouvez les exécuter depuis n'importe quel dossier du projet.

## Utilisation dans le Code C++

### Charger une Image

```cpp
// Créer l'objet image
lv_obj_t* logo = lv_image_create(parent);

// Charger depuis le fichier binaire
lv_image_set_src(logo, "S:/plugin/bitwig/resource/img/bitwig_logo.bin");

// Positionner
lv_obj_align(logo, LV_ALIGN_CENTER, 0, 0);
```

### Avec Transparence

Les images converties avec `ARGB8565` conservent la transparence:

```cpp
// Fond coloré
lv_obj_t* bg = lv_obj_create(parent);
lv_obj_set_style_bg_color(bg, lv_color_hex(0xFF5722), 0);

// Logo avec transparence par-dessus
lv_obj_t* logo = lv_image_create(bg);
lv_image_set_src(logo, "S:/plugin/bitwig/resource/img/bitwig_logo.bin");
lv_obj_center(logo);
```

## Configuration LVGL Requise

Dans `src/config/ui/lv_conf.h`:

```c
#define LV_USE_LZ4_INTERNAL 1  // Pour la décompression LZ4
```

## Formats de Couleur

| Format | Taille | Alpha | Usage |
|--------|--------|-------|-------|
| RGB565 | 2 bytes/px | Non | Images opaques |
| ARGB8565 | 3 bytes/px | Oui | Images transparentes |
| ARGB8888 | 4 bytes/px | Oui | Qualité maximale |
| L8 | 1 byte/px | Non | Niveaux de gris |

## Performance

Pour le logo Bitwig (84x64 pixels):
- Format: ARGB8565 (avec alpha)
- Non compressé: 16,128 bytes
- Avec LZ4: 3,917 bytes (75.7% compression)
- Décompression en RAM lors de l'affichage

## Intégration SVG

Pour intégrer un SVG:
1. Exporter le SVG en PNG à la résolution souhaitée
2. Utiliser `convert_image.py` avec le format approprié
3. Charger le fichier `.bin` résultant

**Pourquoi pas ThorVG?**
Le support SVG natif (ThorVG) nécessite trop de RAM/CPU pour Teensy 4.1.

## Ressources

- **LVGL Documentation**: https://docs.lvgl.io/master/overview/image.html
- **LVGLImage.py**: `.pio/libdeps/debug/lvgl/scripts/LVGLImage.py`
