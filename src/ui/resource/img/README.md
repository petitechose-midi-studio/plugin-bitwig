# Logo Bitwig - Images LVGL

Ce dossier contient le logo Bitwig converti en différents formats LVGL.

## Fichiers Disponibles

### 1. `bitwig_logo.bin` - Chargement depuis Filesystem
- **Format**: ARGB8565 (24-bit avec transparence)
- **Compression**: LZ4
- **Taille**: 3.9 KB
- **Usage**: Chargement dynamique depuis SD/Flash FS

**Code:**
```cpp
lv_obj_t* logo = lv_image_create(parent);
lv_image_set_src(logo, "S:/plugin/bitwig/resource/img/bitwig_logo.bin");
lv_obj_center(logo);
```

### 2. `bitwig_logo.c` - Stockage en Flash (Recommandé)
- **Format**: ARGB8565 (24-bit avec transparence)
- **Compression**: LZ4
- **Taille**: 22 KB (code source)
- **Taille Flash**: ~4 KB (compilé)
- **Usage**: Intégré directement dans le firmware

**Code:**

1. **Inclure le fichier dans votre build**

   Ajoutez dans votre CMakeLists.txt ou platformio.ini :
   ```ini
   build_src_filter = +<plugin/bitwig/resource/img/bitwig_logo.c>
   ```

2. **Déclarer l'image dans votre header (.h)**
   ```cpp
   // bitwig_ui.h
   #include "lvgl.h"

   // Déclaration externe du logo
   LV_IMG_DECLARE(Bitwig_Logo);
   ```

3. **Utiliser dans votre code (.cpp)**
   ```cpp
   // bitwig_ui.cpp
   #include "bitwig_ui.h"

   void show_bitwig_logo(lv_obj_t* parent) {
       // Créer l'objet image
       lv_obj_t* logo = lv_image_create(parent);

       // Charger depuis la flash
       lv_image_set_src(logo, &Bitwig_Logo);

       // Positionner
       lv_obj_center(logo);

       // Optionnel: redimensionner
       lv_image_set_scale(logo, 200);  // 200% = 2x
   }
   ```

## Avantages de chaque méthode

### Flash (fichier .c) - ✅ Recommandé
- ✅ Pas besoin de filesystem
- ✅ Chargement instantané
- ✅ Stocké dans la flash du MCU
- ✅ Pas de risque de fichier manquant
- ❌ Augmente la taille du firmware

### Filesystem (fichier .bin)
- ✅ Ne prend pas de place dans le firmware
- ✅ Peut être mis à jour sans recompiler
- ❌ Nécessite un système de fichiers (SD/LittleFS)
- ❌ Temps de chargement légèrement plus long

## Caractéristiques de l'Image

- **Dimensions**: 84×64 pixels
- **Format**: ARGB8565 (RGB565 + Alpha 8-bit)
- **Transparence**: Oui (canal alpha préservé)
- **Compression**: LZ4 (décompression automatique par LVGL)
- **RAM requise**: ~16 KB lors de l'affichage (décompressé)

## Regénérer les Images

### Logo binaire (filesystem):
```bash
python plugin/bitwig/resource/code/py/lvgl/convert_logo.py
```

### Logo C (flash):
```bash
python plugin/bitwig/resource/code/py/lvgl/convert_logo_to_c.py
```

## Configuration LVGL Requise

Dans `src/config/ui/lv_conf.h`:
```c
#define LV_USE_LZ4_INTERNAL 1  // Pour décompression LZ4
```

## Exemple Complet

```cpp
#include "lvgl.h"

// Déclaration (dans le .h ou avant utilisation)
LV_IMG_DECLARE(Bitwig_Logo);

class BitwigLogoWidget {
private:
    lv_obj_t* logo_obj;

public:
    BitwigLogoWidget(lv_obj_t* parent) {
        // Créer fond avec couleur
        lv_obj_t* bg = lv_obj_create(parent);
        lv_obj_set_size(bg, 100, 80);
        lv_obj_set_style_bg_color(bg, lv_color_hex(0x2196F3), 0);
        lv_obj_center(bg);

        // Logo avec transparence
        logo_obj = lv_image_create(bg);
        lv_image_set_src(logo_obj, &Bitwig_Logo);
        lv_obj_center(logo_obj);
    }

    void set_scale(uint16_t zoom) {
        lv_image_set_scale(logo_obj, zoom);
    }

    void rotate(int16_t angle) {
        lv_image_set_rotation(logo_obj, angle * 10);  // 0.1° units
    }
};
```

---

**Dernière mise à jour**: 2025-01-09
