# =============================================================================
# MIDI Studio Bitwig Plugin - App Configuration
# =============================================================================
# This file is included by CMakeLists.txt via -DAPP_PATH
# All paths are relative to CMAKE_CURRENT_LIST_DIR (this file's directory)

set(APP_ID "bitwig")
set(APP_NAME "MIDI Studio Bitwig")
set(APP_EXE_NAME "midi_studio_bitwig")

# -----------------------------------------------------------------------------
# Source paths (relative to this file)
# -----------------------------------------------------------------------------
set(APP_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/../src")
set(APP_MAIN_NATIVE "${CMAKE_CURRENT_LIST_DIR}/main-native.cpp")
set(APP_MAIN_WASM "${CMAKE_CURRENT_LIST_DIR}/main-wasm.cpp")

# -----------------------------------------------------------------------------
# Additional include directories (relative to this file)
# -----------------------------------------------------------------------------
set(APP_EXTRA_INCLUDES "")

# -----------------------------------------------------------------------------
# Additional sources
# -----------------------------------------------------------------------------
# The shared UI implementation (ms-ui) is already compiled by the common SDL
# build target (see midi-studio/core/sdl/CMakeLists.txt -> SRC_MS_UI).
set(APP_EXTRA_SOURCES "")
