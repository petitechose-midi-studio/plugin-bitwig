#!/bin/bash
# Icon Font Builder - Wrapper for unified builder
# Usage: ./generate_icons.sh

set -e

# Get script directory and derive paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# plugin-bitwig/script/icons -> plugin-bitwig -> midi-studio
PLUGIN_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
MIDI_STUDIO_ROOT="$(dirname "$PLUGIN_ROOT")"
BUILDER="$MIDI_STUDIO_ROOT/script/icon/builder.py"

# Check builder exists
[[ -f "$BUILDER" ]] || { echo "Builder not found: $BUILDER" >&2; exit 1; }

# Run unified builder for plugin-bitwig
python "$BUILDER" plugin-bitwig "$@"
