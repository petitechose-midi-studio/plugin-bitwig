/**
 * @file main-wasm.cpp
 * @brief WebAssembly entry point for MIDI Studio Bitwig Plugin (Browser)
 *
 * Uses emscripten_set_main_loop for browser-compatible event loop.
 * Note: Remote transport (UDP) is not available in WASM.
 */

#include "SdlEnvironment.hpp"

#include "entry/MidiDefaults.hpp"
#include "entry/SdlRunLoop.hpp"
#include "entry/BridgeArgs.hpp"
#include "entry/WasmArgs.hpp"

#include <oc/hal/sdl/Sdl.hpp>
#include <oc/hal/midi/LibreMidiTransport.hpp>
#include <oc/hal/net/WebSocketTransport.hpp>

#include <config/App.hpp>
#include "app/AppLogic.hpp"

int main(int argc, char** argv) {
    // Static storage for WASM (persists across main loop iterations)
    static sdl::SdlEnvironment env;

    if (!env.init(argc, argv)) {
        return 1;
    }

    const auto midi = ms::wasm::parse_midi_args(argc, argv);
    const auto ws_url = ms::bridge::ws_url(argc, argv, "ws://localhost:8101");

    // Build application with MIDI + WebSocket (for oc-bridge communication)
    // Connects to bridge's controller WebSocket server (default: 8101, override via --bridge-ws-url)
    // WebMIDI: searches for existing ports matching pattern (no virtual port creation)
    static oc::app::OpenControlApp app = oc::hal::sdl::AppBuilder()
        .midi(std::make_unique<oc::hal::midi::LibreMidiTransport>(
            ms::midi::make_wasm_config("MIDI Studio WASM", midi.in, midi.out)))
        .remote(std::make_unique<oc::hal::net::WebSocketTransport>(
            oc::hal::net::WebSocketConfig{
                .url = ws_url  // Controller: bitwig wasm (configurable via --bridge-ws-url)
            }))
        .controllers(env.inputMapper())
        .inputConfig(Config::Input::CONFIG);

    // Register main context only (skip splash for WASM)
    // Note: Contexts use Screen::root() which is configured to HwSimulator's screenArea
    app.registerContext<bitwig::BitwigContext>(bitwig::ContextID::BITWIG, "Bitwig");
    app.begin();

    return ms::entry::run_wasm(env, app);
}
