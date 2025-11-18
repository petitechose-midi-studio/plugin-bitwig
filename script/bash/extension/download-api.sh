#!/bin/bash

# Script multiplateforme pour télécharger l'API Bitwig
API_VERSION=25

# Détecter la racine du projet (où se trouve platformio.ini)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

# Créer l'arborescence dans host/ depuis la racine du projet
API_DIR="$PROJECT_ROOT/host/api/$API_VERSION"

echo "=========================================="
echo "   Download Bitwig API v$API_VERSION"
echo "=========================================="
echo ""
echo "📂 Project root: $PROJECT_ROOT"
echo "📂 API directory: $API_DIR"
echo ""

# Créer le dossier si nécessaire
mkdir -p "$API_DIR"

# URLs des fichiers
JAR_URL="https://maven.bitwig.com/com/bitwig/extension-api/$API_VERSION/extension-api-$API_VERSION.jar"
SOURCES_URL="https://maven.bitwig.com/com/bitwig/extension-api/$API_VERSION/extension-api-$API_VERSION-sources.jar"

# Fonction de téléchargement multiplateforme
download_file() {
    local url="$1"
    local output="$2"
    local name="$3"

    echo "📥 Downloading $name..."

    # Essayer curl d'abord (disponible sur Unix et Windows 10+)
    if command -v curl >/dev/null 2>&1; then
        curl -L -f --show-error --progress-bar "$url" -o "$output"
    # Fallback sur wget (Linux)
    elif command -v wget >/dev/null 2>&1; then
        wget --progress=bar "$url" -O "$output"
    # Fallback sur PowerShell (Windows)
    elif command -v powershell >/dev/null 2>&1; then
        powershell -Command "Invoke-WebRequest -Uri '$url' -OutFile '$output'"
    else
        echo "❌ Error: No download tool found (curl, wget, or powershell)"
        echo "   Please install curl or wget to continue."
        exit 1
    fi

    if [ $? -eq 0 ]; then
        echo "   ✅ $name downloaded successfully"
    else
        echo "   ❌ Failed to download $name"
        exit 1
    fi
}

# Télécharger les fichiers
download_file "$JAR_URL" "$API_DIR/extension-api-$API_VERSION.jar" "API JAR"
download_file "$SOURCES_URL" "$API_DIR/extension-api-$API_VERSION-sources.jar" "Sources JAR"

echo ""
echo "📦 Extracting API JARs..."

# Créer les dossiers d'extraction
CLASSES_DIR="$API_DIR/classes"
SOURCES_DIR="$API_DIR/sources"
mkdir -p "$CLASSES_DIR" "$SOURCES_DIR"

# Extraire le JAR principal
if command -v unzip >/dev/null 2>&1; then
    echo "   Extracting classes JAR..."
    unzip -q -o "$API_DIR/extension-api-$API_VERSION.jar" -d "$CLASSES_DIR"
    echo "   ✅ Classes extracted to $CLASSES_DIR"

    echo "   Extracting sources JAR..."
    unzip -q -o "$API_DIR/extension-api-$API_VERSION-sources.jar" -d "$SOURCES_DIR"
    echo "   ✅ Sources extracted to $SOURCES_DIR"
elif command -v jar >/dev/null 2>&1; then
    echo "   Extracting classes JAR (using jar)..."
    (cd "$CLASSES_DIR" && jar xf "../extension-api-$API_VERSION.jar")
    echo "   ✅ Classes extracted to $CLASSES_DIR"

    echo "   Extracting sources JAR (using jar)..."
    (cd "$SOURCES_DIR" && jar xf "../extension-api-$API_VERSION-sources.jar")
    echo "   ✅ Sources extracted to $SOURCES_DIR"
else
    echo "   ⚠️  Warning: No extraction tool found (unzip or jar)"
    echo "   JARs downloaded but not extracted. Install unzip to extract automatically."
fi

echo ""
echo "🎉 Download completed!"
echo "   Files saved in: $API_DIR"
echo "   • extension-api-$API_VERSION.jar"
echo "   • extension-api-$API_VERSION-sources.jar"
echo "   • classes/ (extracted)"
echo "   • sources/ (extracted)"