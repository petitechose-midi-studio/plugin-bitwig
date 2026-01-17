#!/bin/bash
#
# Protocol Code Generator for Bitwig MIDI Studio Plugin
#
# Generates C++ and Java protocol code from SysEx message definitions
# using the standalone protocol-codegen package via uvx.
#
# Usage:
#   ./script/protocol/generate_protocol.sh
#
# Requirements:
#   - uv/uvx installed (https://docs.astral.sh/uv/)
#

set -e  # Exit on error

# --- Find project root (looks for platformio.ini) ---
find_root() {
    local dir="$(pwd)"
    while [[ "$dir" != "/" ]]; do
        [[ -f "$dir/platformio.ini" ]] && echo "$dir" && return
        dir="$(dirname "$dir")"
    done
    echo "$(pwd)"
}

PROJECT_ROOT="$(find_root)"

# --- Colors ---
RED='\033[0;31m'
GREEN='\033[0;32m'
BOLD='\033[1m'
NC='\033[0m'

echo "==================================="
echo "Protocol Code Generator"
echo "==================================="
echo "Project: Bitwig MIDI Studio Plugin"
echo "Root: $PROJECT_ROOT"
echo "==================================="

# Verify we found a valid project
if [[ ! -f "$PROJECT_ROOT/platformio.ini" ]]; then
    echo -e "${RED}Error: platformio.ini not found. Run from project directory.${NC}"
    exit 1
fi

# Check if uvx is available
if ! command -v uvx &> /dev/null; then
    echo -e "${RED}Error: uvx not found. Please install uv:${NC}"
    echo "  curl -LsSf https://astral.sh/uv/install.sh | sh"
    exit 1
fi

cd "$PROJECT_ROOT"

echo "Running protocol-codegen..."
echo ""

# Run protocol-codegen via uv run (uses local editable install from pyproject.toml)
uv run protocol-codegen generate \
    --method binary \
    --messages "protocol/message" \
    --config "protocol/binary_protocol_config.py" \
    --plugin-paths "protocol/plugin_paths.py" \
    --output-base "." \
    --verbose

echo ""
echo -e "${GREEN}===================================${NC}"
echo -e "${GREEN}Protocol generation complete!${NC}"
echo -e "${GREEN}===================================${NC}"
echo "Generated files:"
echo "  C++:  src/protocol/"
echo "  Java: host/src/protocol/"
echo "==================================="
