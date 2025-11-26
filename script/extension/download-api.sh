#!/bin/bash
# Download Bitwig Extension API

API_VERSION=25

find_root() {
    local dir="$(pwd)"
    while [[ "$dir" != "/" ]]; do
        [[ -f "$dir/platformio.ini" ]] && echo "$dir" && return
        dir="$(dirname "$dir")"
    done
    echo "$(pwd)"
}

PROJECT_ROOT="$(find_root)"
API_DIR="$PROJECT_ROOT/host/api/$API_VERSION"

echo "═══ Download Bitwig API v$API_VERSION ═══"
echo "📂 Project: $PROJECT_ROOT"
echo "📂 API: $API_DIR"

mkdir -p "$API_DIR"

JAR_URL="https://maven.bitwig.com/com/bitwig/extension-api/$API_VERSION/extension-api-$API_VERSION.jar"
SOURCES_URL="https://maven.bitwig.com/com/bitwig/extension-api/$API_VERSION/extension-api-$API_VERSION-sources.jar"

download() {
    local url="$1" output="$2" name="$3"
    echo "📥 $name..."
    if command -v curl &>/dev/null; then
        curl -L -f --progress-bar "$url" -o "$output"
    elif command -v wget &>/dev/null; then
        wget -q --show-progress "$url" -O "$output"
    else
        echo "❌ curl/wget not found"; exit 1
    fi
    echo "   ✅ $name"
}

download "$JAR_URL" "$API_DIR/extension-api-$API_VERSION.jar" "API JAR"
download "$SOURCES_URL" "$API_DIR/extension-api-$API_VERSION-sources.jar" "Sources JAR"

echo "📦 Extracting..."
CLASSES_DIR="$API_DIR/classes"
SOURCES_DIR="$API_DIR/sources"
mkdir -p "$CLASSES_DIR" "$SOURCES_DIR"

if command -v unzip &>/dev/null; then
    unzip -q -o "$API_DIR/extension-api-$API_VERSION.jar" -d "$CLASSES_DIR"
    unzip -q -o "$API_DIR/extension-api-$API_VERSION-sources.jar" -d "$SOURCES_DIR"
    echo "   ✅ Extracted"
elif command -v jar &>/dev/null; then
    (cd "$CLASSES_DIR" && jar xf "../extension-api-$API_VERSION.jar")
    (cd "$SOURCES_DIR" && jar xf "../extension-api-$API_VERSION-sources.jar")
    echo "   ✅ Extracted"
else
    echo "   ⚠️ unzip/jar not found, skipping extraction"
fi

echo "🎉 Done! Files in: $API_DIR"
