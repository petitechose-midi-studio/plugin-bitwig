#include <Arduino.h>
#include "log/Macros.hpp"

#include "app/MidiStudioApp.hpp"
#include "Plugin.hpp"

void setupPlugins(PluginManager& manager) {
    manager.registerPlugin<Bitwig::Plugin>("bitwig");
}

MidiStudioApp app(setupPlugins);

void setup() {
    waitForSerial();
    LOGLN("=== MIDI Studio - Bitwig ===");
    LOG("Version: ");
    LOGLN(Core::VERSION);
    app.setup();
}

void loop() {
    app.update();
}
