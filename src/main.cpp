#include <Arduino.h>

#include "app/MidiStudioApp.hpp"
#include "log/Macros.hpp"
#include "../embedded/Plugin.hpp"

// Plugin registration callback
void setupPlugins(PluginManager& manager) {
    manager.registerPlugin<Bitwig::Plugin>("bitwig");
}

// Instantiate app with Bitwig plugin
MidiStudioApp app(setupPlugins);

void setup() {
    delay(200);
    LOGLN("=======================================");
    LOGLN("=== MIDI Studio - Bitwig Plugin Dev ===");
    LOGLN("=======================================");
    LOG("Core Version : ");
    LOGLN(Core::VERSION);
    LOGLN("=======================================");
    LOGLN("============= System Boot =============");
    LOGLN("=======================================");
}

void loop() {
    app.update();
}
