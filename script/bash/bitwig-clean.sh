#!/bin/bash
# ============================================================================
# Bitwig Extension - Maven Clean
# ============================================================================
# Usage: ./plugin/bitwig/script/bash/bitwig-clean.sh

cd "$(dirname "$0")/../../host"
mvn clean
