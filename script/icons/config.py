# Icon Font Builder Configuration

# Paths (relative to project root)
SVG_SOURCE_DIR = "asset/icon"
TTF_OUTPUT_DIR = "asset/font"
HEADER_OUTPUT_DIR = "src/ui/font"
CACHE_DIR = ".cache/icons"

# Tools
# Linux (default):
# INKSCAPE = "inkscape"
# FONTFORGE = "fontforge"

# Windows:
INKSCAPE = "C:/Program Files/Inkscape/bin/inkscape.exe"
FONTFORGE = "C:/Program Files/FontForgeBuilds/bin/fontforge.exe"

# lv_font_conv: installed globally via npm (npm i -g lv_font_conv)

# Font settings
FONT_NAME = "bitwig_icons"
FONT_FAMILY = "Bitwig Icons"
UNITS_PER_EM = 1000
ASCENT = 800
DESCENT = 200
GLYPH_MARGIN = 50
UNICODE_START = 0xE000

# SVG processing
PADDING_PERCENT = 0.10

# LVGL font generation
FONT_SIZES = {'S': 12, 'M': 14, 'L': 16}  # Icon::Size enum: name → pixels
LVGL_BPP = 4  # Bits per pixel (1, 2, 4, or 8)

# Header include (file that declares bitwig_fonts global)
HEADER_INCLUDE = "BitwigFonts.hpp"
