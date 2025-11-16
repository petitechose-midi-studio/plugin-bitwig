#!/bin/bash
# ============================================================================
# Bitwig Extension - Maven Package & Deploy
# ============================================================================
# Usage: ./plugin/bitwig/script/bash/bitwig-package.sh

cd "$(dirname "$0")/../../host"
mvn package
