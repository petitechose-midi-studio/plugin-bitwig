/**
 * @file main-native.cpp
 * @brief Native entry point for MIDI Studio Bitwig Plugin (Windows/Linux/macOS)
 *
 * Unlike core, Bitwig plugin:
 * - Has no external state (BitwigState is internal to BitwigContext)
 * - Requires remote transport (UDP) to communicate with oc-bridge -> Bitwig
 *
 * Note: Skip boot/splash context for desktop builds (instant startup).
 */

#define SDL_MAIN_HANDLED
#include "SdlEnvironment.hpp"

#include <oc/hal/sdl/Sdl.hpp>
#include <oc/hal/midi/LibreMidiTransport.hpp>
#include <oc/hal/net/UdpTransport.hpp>

#include <config/App.hpp>
#include "app/AppLogic.hpp"

int main(int argc, char** argv) {
    // 1. Initialize SDL environment
    sdl::SdlEnvironment env;
    if (!env.init(argc, argv)) {
        return 1;
    }

    // 2. Build application with MIDI + remote transport
    // MIDI port strategy:
    //   - Linux: Connect to VirMIDI kernel ports (snd-virmidi module)
    //   - macOS: Create virtual ports (CoreMIDI supports this natively)
    //   - Windows: Connect to loopMIDI ports (user creates once)
    oc::app::OpenControlApp app = oc::hal::sdl::AppBuilder()
        .midi(std::make_unique<oc::hal::midi::LibreMidiTransport>(
            oc::hal::midi::LibreMidiConfig{
                .appName = "MIDI Studio Native",
#if defined(__linux__)
                .inputPortName = "VirMIDI",    // Connect to VirMIDI kernel ports
                .outputPortName = "VirMIDI",
                .useVirtualPorts = false
#elif defined(__APPLE__)
                .inputPortName = "MIDI Studio [native] IN",
                .outputPortName = "MIDI Studio [native] OUT",
                .useVirtualPorts = true        // macOS: CoreMIDI virtual ports
#else
                .inputPortName = "loopMIDI",   // Windows: loopMIDI virtual ports
                .outputPortName = "loopMIDI",
                .useVirtualPorts = false
#endif
            }))
        .remote(std::make_unique<oc::hal::net::UdpTransport>(
            oc::hal::net::UdpConfig{
                .host = "127.0.0.1",
                .port = 8001  // Controller: bitwig native (host: 9001)
            }))
        .controllers(env.inputMapper())
        .inputConfig(Config::Input::CONFIG);

    // 3. Register main context only (skip splash for desktop)
    // Note: Contexts use Screen::root() which is configured to HwSimulator's screenArea
    app.registerContext<bitwig::BitwigContext>(bitwig::ContextID::BITWIG, "Bitwig");
    app.begin();

    // 4. Main loop (no external state to update)
    while (env.processEvents()) {
        app.update();
        env.refresh();
    }

    // Cleanup: handled by destructors in correct order
    //    (app destroyed first, then env calls SDL_Quit)
    return 0;
}
