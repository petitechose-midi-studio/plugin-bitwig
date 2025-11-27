# Bitwig Icon Font Generator

Generates a custom icon font from SVG files for use with LVGL.

## Quick Start

```bash
# 1. Add SVGs to asset/icon/
# 2. Generate TTF + header + LVGL binaries
bash script/font/generate_bitwig_icons.sh
```

## How It Works

All `.svg` files in `asset/icon/` are automatically scanned and converted:

```
asset/icon/
├── transport_play.svg    → Icon::TRANSPORT_PLAY
├── transport_stop.svg    → Icon::TRANSPORT_STOP
├── arrow_left.svg        → Icon::ARROW_LEFT
└── subfolder/
    └── my_icon.svg       → Icon::MY_ICON
```

## Output Files

| File | Description |
|------|-------------|
| `asset/font/bitwig_icons.ttf` | TrueType font |
| `src/ui/font/icon.hpp` | C++ constants + `Icon::set()` |
| `src/ui/font/data/bitwig_icons_*.c.inc` | LVGL binary (12, 14, 18px) |

## Usage in Code

```cpp
#include "ui/font/icon.hpp"

// Simple usage (default 14px)
Icon::set(label, Icon::TRANSPORT_PLAY);

// With size
Icon::set(label, Icon::TRANSPORT_PLAY, Icon::S12);
Icon::set(label, Icon::TRANSPORT_PLAY, Icon::S18);
```

## Adding New Icons

1. Add SVG file to `asset/icon/` (filename = constant name)
2. Run `bash script/font/generate_bitwig_icons.sh`
3. Use: `Icon::YOUR_ICON_NAME`

## SVG Requirements

- Single color (fill only, no stroke)
- Simple paths (no gradients, effects, or text)
- Square viewBox recommended (e.g., `viewBox="0 0 24 24"`)
