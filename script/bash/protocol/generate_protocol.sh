#!/bin/bash
#
# Protocol Code Generator for Bitwig MIDI Studio Plugin
#
# Generates C++ and Java protocol code from SysEx message definitions
# using the standalone protocol-codegen package via uvx.
#
# Usage:
#   ./scripts/generate_protocol.sh
#
# Requirements:
#   - uv/uvx installed (https://docs.astral.sh/uv/)
#

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

echo "==================================="
echo "Protocol Code Generator"
echo "==================================="
echo "Project: Bitwig MIDI Studio Plugin"
echo "Root: $PROJECT_ROOT"
echo "==================================="

cd "$PROJECT_ROOT"

# Check if uvx is available
if ! command -v uvx &> /dev/null; then
    echo "Error: uvx not found. Please install uv:"
    echo "  curl -LsSf https://astral.sh/uv/install.sh | sh"
    exit 1
fi

echo "Running protocol-codegen..."
echo ""

# Run protocol-codegen via uvx from GitHub
# This will download and cache the package automatically
uvx --python python3.13 --from git+https://github.com/petitechose-audio/protocol-codegen \
    protocol-codegen generate \
    --method sysex \
    --messages "$PROJECT_ROOT/protocol/message" \
    --config "$PROJECT_ROOT/protocol/sysex_protocol_config.py" \
    --plugin-paths "$PROJECT_ROOT/protocol/plugin_paths.py" \
    --output-base "$PROJECT_ROOT" \
    --verbose

echo ""
echo "==================================="
echo "Protocol generation complete!"
echo "==================================="
echo "Generated files:"
echo "  C++:  embedded/protocol/"
echo "  Java: host/src/main/java/com/midi_studio/protocol/"
echo "==================================="
