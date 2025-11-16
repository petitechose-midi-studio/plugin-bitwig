#!/bin/bash
# ============================================================================
# Bitwig Extension - Maven Compile
# ============================================================================
# Usage: ./plugin/bitwig/script/bash/bitwig-compile.sh

cd "$(dirname "$0")/../../host"
mvn compile
