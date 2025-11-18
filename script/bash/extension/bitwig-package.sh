#!/bin/bash
# ============================================================================
# Bitwig Extension - Maven Package & Deploy
# ============================================================================
# Usage: ./plugin/bitwig/script/bash/bitwig-package.sh

# Masquer les warnings Guice/Unsafe de Maven (temporaire jusqu'à Maven 3.9.12)
export MAVEN_OPTS="--sun-misc-unsafe-memory-access=allow"

# Détecter la racine du projet
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
HOST_DIR="$PROJECT_ROOT/host"

echo "📂 Project root: $PROJECT_ROOT"
echo "📂 Host directory: $HOST_DIR"
echo ""

cd "$HOST_DIR"
mvn package
