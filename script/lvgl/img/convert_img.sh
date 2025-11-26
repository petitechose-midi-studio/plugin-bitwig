#!/bin/bash
# Image Converter Wrapper - Calls convert_img.sh from core

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

PATHS=(
    "$PROJECT_ROOT/../core/script/lvgl/img/convert_img.sh"
    "$PROJECT_ROOT/.pio/libdeps/"*/petitechose-midi-studio-core/script/lvgl/img/convert_img.sh
)

CONVERTER=""
for p in "${PATHS[@]}"; do
    for f in $p; do [[ -f $f ]] && { CONVERTER="$f"; break 2; }; done
done

[[ -z $CONVERTER ]] && { echo -e "\033[0;31m✗\033[0m convert_img.sh not found"; exit 1; }

cd "$PROJECT_ROOT" && exec bash "$CONVERTER"
