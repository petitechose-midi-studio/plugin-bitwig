# Bitwig Icon Font Generator

Generates a custom icon font from SVG files for use with LVGL.

## Quick Start

```bash
# 1. Add SVGs to asset/icon/
# 2. Generate TTF + header
bash script/font/generate_bitwig_icons.sh

# 3. Convert to LVGL binary
bash script/lvgl/font/convert_font.sh
```

## How It Works

All `.svg` files in `asset/icon/` are automatically scanned and converted:

```
asset/icon/
├── transport_play.svg    → BitwigIcon::TRANSPORT_PLAY
├── transport_stop.svg    → BitwigIcon::TRANSPORT_STOP
├── arrow_left.svg        → BitwigIcon::ARROW_LEFT
└── subfolder/
    └── my_icon.svg       → BitwigIcon::MY_ICON
```

## Output Files

| File | Description |
|------|-------------|
| `asset/font/bitwig_icons.ttf` | TrueType font |
| `src/ui/font/bitwig_icons.hpp` | C++ constants |
| `src/ui/font/data/bitwig_icons_14.*` | LVGL binary (after conversion) |

## Usage in Code

```cpp
#include "ui/font/bitwig_icons.hpp"
#include "ui/font/data/bitwig_icons_14.hpp"

// Load font
auto* iconFont = lv_binfont_create_from_buffer(
    bitwig_icons_14_bin,
    bitwig_icons_14_bin_len
);

// Use icon
lv_label_set_text(label, BitwigIcon::TRANSPORT_PLAY);
```

## Adding New Icons

1. Add SVG file to `asset/icon/` (filename = constant name)
2. Run `bash script/font/generate_bitwig_icons.sh`
3. Run `bash script/lvgl/font/convert_font.sh`
4. Use: `BitwigIcon::YOUR_ICON_NAME`

## SVG Requirements

- Single color (fill only, no stroke)
- Simple paths (no gradients, effects, or text)
- Square viewBox recommended (e.g., `viewBox="0 0 24 24"`)

## Configuration (optional)

`script/font/bitwig_icons.conf`:
```bash
SVG_SOURCE_DIR="asset/icon"   # SVG source (default)
TTF_NAME="bitwig_icons"       # Output name (default)
```
