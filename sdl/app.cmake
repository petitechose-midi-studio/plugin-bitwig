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
# Features
# -----------------------------------------------------------------------------
set(APP_HAS_CORE_STATE OFF)     # No CoreState, BitwigContext has its own state
set(APP_USE_MIDI ON)            # MIDI transport
set(APP_USE_UDP ON)             # UDP transport for oc-bridge
set(APP_USE_WEBSOCKET OFF)      # WebSocket for WASM (future)

# -----------------------------------------------------------------------------
# Additional include directories (relative to this file)
# -----------------------------------------------------------------------------
set(APP_EXTRA_INCLUDES "")

# -----------------------------------------------------------------------------
# Core sources needed by this plugin
# -----------------------------------------------------------------------------
set(CORE_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/../../core/src")
set(APP_EXTRA_SOURCES
    "${CORE_SRC_DIR}/ui/font/CoreFonts.cpp"
    "${CORE_SRC_DIR}/ui/ViewContainer.cpp"
)
