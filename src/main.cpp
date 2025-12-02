#include "Plugin.hpp"

#include <Arduino.h>

#include "app/MidiStudioApp.hpp"
#include "log/Macros.hpp"

void setupPlugins(PluginManager& manager) { manager.registerPlugin<Bitwig::Plugin>("bitwig"); }

MidiStudioApp app(setupPlugins);

void setup() {
    LOGLN("=== MIDI Studio - Bitwig ===");
    LOG("Version: ");
    LOGLN(Core::VERSION);
    app.setup();
}

void loop() { app.update(); }
