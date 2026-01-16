/**
 * @file main-wasm.cpp
 * @brief WebAssembly entry point for MIDI Studio Bitwig Plugin (Browser)
 *
 * Uses emscripten_set_main_loop for browser-compatible event loop.
 * Note: Remote transport (UDP) is not available in WASM.
 */

#include "SdlEnvironment.hpp"

#include <oc/hal/sdl/Sdl.hpp>
#include <oc/hal/midi/LibreMidiTransport.hpp>
#include <oc/hal/net/WebSocketTransport.hpp>

#include <config/App.hpp>
#include "app/AppLogic.hpp"

#include <emscripten.h>

// Global state for emscripten main loop callback
static sdl::SdlEnvironment* g_env = nullptr;
static oc::app::OpenControlApp* g_app = nullptr;

static void tick(void*) {
    if (!g_env->processEvents()) {
        emscripten_cancel_main_loop();
        return;
    }
    g_app->update();
    g_env->refresh();
}

int main(int argc, char** argv) {
    // Static storage for WASM (persists across main loop iterations)
    static sdl::SdlEnvironment env;

    if (!env.init(argc, argv)) {
        return 1;
    }

    // Build application with MIDI + WebSocket (for oc-bridge communication)
    // Connects to bridge's controller WebSocket server on port 8001
    static oc::app::OpenControlApp app = oc::hal::sdl::AppBuilder()
        .midi(std::make_unique<oc::hal::midi::LibreMidiTransport>(
            oc::hal::midi::LibreMidiConfig{
                .appName = "MIDI Studio Bitwig WASM"
            }))
        .remote(std::make_unique<oc::hal::net::WebSocketTransport>(
            oc::hal::net::WebSocketConfig{
                .url = "ws://localhost:8001"
            }))
        .controllers(env.inputMapper())
        .inputConfig(Config::Input::CONFIG);

    // Register main context only (skip splash for WASM)
    // Note: Contexts use Screen::root() which is configured to HwSimulator's screenArea
    app.registerContext<bitwig::BitwigContext>(bitwig::ContextID::BITWIG, "Bitwig");
    app.begin();

    // Set globals for callback
    g_env = &env;
    g_app = &app;

    // Start emscripten main loop (-1 = use requestAnimationFrame)
    emscripten_set_main_loop_arg(tick, nullptr, -1, true);
    return 0;
}
