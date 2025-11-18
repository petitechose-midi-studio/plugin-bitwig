#!/bin/bash
# ============================================================================
# Bitwig Extension - Maven Compile
# ============================================================================
# Usage: ./plugin/bitwig/script/bash/bitwig-compile.sh

# Détecter la racine du projet
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
HOST_DIR="$PROJECT_ROOT/host"

echo "📂 Project root: $PROJECT_ROOT"
echo "📂 Host directory: $HOST_DIR"
echo ""

cd "$HOST_DIR"
mvn compile
