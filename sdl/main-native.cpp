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

#include "entry/MidiDefaults.hpp"
#include "entry/SdlRunLoop.hpp"

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

    oc::app::OpenControlApp app = oc::hal::sdl::AppBuilder()
        .midi(std::make_unique<oc::hal::midi::LibreMidiTransport>(
            ms::midi::make_native_config("MIDI Studio Native")))
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

    return ms::entry::run_native(env, app);
}
