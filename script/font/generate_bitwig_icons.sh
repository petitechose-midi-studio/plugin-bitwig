#!/bin/bash
# Generate Bitwig Icons TTF from all SVG files in source directory

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

# --- Load configs ---
FONT_CONF="$SCRIPT_DIR/../lvgl/font/font_converter.conf"
ICONS_CONF="$SCRIPT_DIR/bitwig_icons.conf"

[[ -f $FONT_CONF ]] || { echo "Config not found: $FONT_CONF" >&2; exit 1; }
source "$FONT_CONF"
[[ -f $ICONS_CONF ]] && source "$ICONS_CONF"

# --- Defaults ---
SVG_SOURCE_DIR="${SVG_SOURCE_DIR:-asset/icon}"
TTF_NAME="${TTF_NAME:-bitwig_icons}"

# --- Derived paths ---
OUTPUT_DIR="$PROJECT_ROOT/$FONT_SOURCE_DIR"
OUTPUT_FILE="$OUTPUT_DIR/${TTF_NAME}.ttf"
HEADER_OUTPUT="$PROJECT_ROOT/$FONT_OUTPUT_DIR/icon.hpp"

# --- Colors ---
GREEN='\033[0;32m' BLUE='\033[0;34m' RED='\033[0;31m' DIM='\033[2m' NC='\033[0m'

log()     { echo -e "${BLUE}●${NC} $1"; }
success() { echo -e "  ${GREEN}✓${NC} $1"; }
error()   { echo -e "${RED}✗${NC} $1" >&2; exit 1; }

# Count SVGs
SVG_COUNT=$(find "$PROJECT_ROOT/$SVG_SOURCE_DIR" -name "*.svg" 2>/dev/null | wc -l)

echo -e "\n${BLUE}═══ Bitwig Icon Font Generator ═══${NC}"
echo -e "${DIM}Source: $SVG_SOURCE_DIR${NC}"
echo -e "${DIM}Found: $SVG_COUNT SVG files${NC}"

[[ $SVG_COUNT -eq 0 ]] && error "No SVG files found in $SVG_SOURCE_DIR"

# --- Export for Python ---
export SVG_SOURCE_DIR="$PROJECT_ROOT/$SVG_SOURCE_DIR"
export TTF_OUTPUT="$SCRIPT_DIR/${TTF_NAME}.ttf"
export HEADER_OUTPUT

# --- Generate TTF + header ---
cd "$PROJECT_ROOT"

RANGE=$(uv run python script/font/generate_icon_font.py)
[[ -z $RANGE ]] && error "Failed to get unicode range"

[[ ! -f "$TTF_OUTPUT" ]] && error "TTF not generated"

# --- Move to font source directory ---
mkdir -p "$OUTPUT_DIR"
mv "$TTF_OUTPUT" "$OUTPUT_FILE"

success "TTF: $OUTPUT_FILE"
success "Header: $HEADER_OUTPUT"
success "Range: $RANGE"

echo -e "\n${GREEN}✓ Done!${NC}"
echo -e "Run ${BLUE}./script/lvgl/font/convert_font.sh${NC} to convert to LVGL format"
