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
    LOGLN("=== MIDI Studio - Bitwig Plugin ===");
    Serial.print("Version: ");
    Serial.println(Core::VERSION);
    LOGLN("=== System Ready ===");
}

void loop() {
    app.update();
}
