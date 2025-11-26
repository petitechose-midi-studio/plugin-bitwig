#!/bin/bash
# Bitwig Extension - Maven Compile

export MAVEN_OPTS="--sun-misc-unsafe-memory-access=allow"

find_root() {
    local dir="$(pwd)"
    while [[ "$dir" != "/" ]]; do
        [[ -f "$dir/platformio.ini" ]] && echo "$dir" && return
        dir="$(dirname "$dir")"
    done
    echo "$(pwd)"
}

PROJECT_ROOT="$(find_root)"
HOST_DIR="$PROJECT_ROOT/host"

echo "📂 Project: $PROJECT_ROOT"
echo "📂 Host: $HOST_DIR"

cd "$HOST_DIR" && mvn compile
