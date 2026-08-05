#!/bin/bash
# Image Converter Wrapper - Calls convert_img.sh from core

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

CONVERTER="$PROJECT_ROOT/../core/script/lvgl/img/convert_img.sh"

[[ ! -f "$CONVERTER" ]] && { echo -e "\033[0;31m✗\033[0m convert_img.sh not found"; exit 1; }

cd "$PROJECT_ROOT" && exec bash "$CONVERTER"
