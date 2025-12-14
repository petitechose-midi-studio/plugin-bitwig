/**
 * @file main.cpp
 * @brief MIDI Studio - Bitwig Plugin
 *
 * Uses oc::teensy::AppBuilder for Teensy 4.1 setup.
 * Registers BitwigContext as the main (and only) context.
 */

#include <optional>

#include <Arduino.h>
#include <oc/teensy/Teensy.hpp>

#include "config/App.hpp"
#include "config/Buffer.hpp"
#include "config/Hardware.hpp"
#include "context/BitwigContext.hpp"

// =============================================================================
// Debug: Memory monitoring for crash diagnosis
// =============================================================================
extern "C" char* sbrk(int incr);

static uint32_t getFreeRAM() {
    char top;
    return &top - reinterpret_cast<char*>(sbrk(0));
}

// =============================================================================
// Context ID for Bitwig
// =============================================================================

enum class BitwigContextID : uint8_t {
    BITWIG = 0,
};

// =============================================================================
// Static Objects
// =============================================================================

static std::optional<oc::teensy::Ili9341> display;
static std::optional<oc::ui::lvgl::Bridge> lvgl;
static std::optional<oc::teensy::CD74HC4067> mux;
static std::optional<oc::app::OpenControlApp> app;

// =============================================================================
// Initialization Helpers
// =============================================================================

static void checkOrHalt(const oc::core::Result<void>& result, const char* component) {
    if (!result) {
        OC_LOG_ERROR("{} init failed: {}", component,
                     oc::core::errorCodeToString(result.error().code));
        while (true) {}
    }
}

static void initDisplay() {
    display = oc::teensy::Ili9341(
        Hardware::Display::CONFIG,
        {.framebuffer = Buffer::framebuffer, .diff1 = Buffer::diff1, .diff2 = Buffer::diff2});
    checkOrHalt(display->init(), "Display");
}

static void initLVGL() {
    lvgl = oc::ui::lvgl::Bridge(*display, Buffer::lvgl, oc::teensy::defaultTimeProvider,
                                 Hardware::LVGL::CONFIG);
    checkOrHalt(lvgl->init(), "LVGL");
}

static void initMux() {
    mux = oc::teensy::CD74HC4067(Hardware::Mux::CONFIG, oc::teensy::gpio());
    checkOrHalt(mux->init(), "MUX");
}

static void initApp() {
    OC_LOG_INFO("[Boot]   4.1: Building app with AppBuilder...");
    app = oc::teensy::AppBuilder()
              .midi()
              .encoders(Hardware::Encoder::ENCODERS)
              .buttons(Hardware::Button::BUTTONS, *mux, Config::Timing::DEBOUNCE_MS)
              .inputConfig(Config::Input::CONFIG);
    OC_LOG_INFO("[Boot]   4.2: AppBuilder complete");

    // Register BitwigContext as the only context (first registered = default)
    OC_LOG_INFO("[Boot]   4.3: Registering BitwigContext...");
    app->registerContext<bitwig::BitwigContext>(BitwigContextID::BITWIG, "Bitwig");
    OC_LOG_INFO("[Boot]   4.4: Context registered");

    OC_LOG_INFO("[Boot]   4.5: Calling app->begin()...");
    app->begin();
    OC_LOG_INFO("[Boot]   4.6: app->begin() complete");
}

// =============================================================================
// Arduino Entry Points
// =============================================================================

void setup() {
    OC_LOG_INFO("========================================");
    OC_LOG_INFO("MIDI Studio - Bitwig Plugin");
    OC_LOG_INFO("App {}Hz, LVGL {}Hz", Config::Timing::APP_HZ, Config::Timing::LVGL_HZ);
    OC_LOG_INFO("========================================");

    OC_LOG_INFO("[Boot] Phase 1: Display init...");
    initDisplay();
    OC_LOG_INFO("[Boot] Phase 1: Display OK");

    OC_LOG_INFO("[Boot] Phase 2: LVGL init...");
    initLVGL();
    OC_LOG_INFO("[Boot] Phase 2: LVGL OK");

    OC_LOG_INFO("[Boot] Phase 3: MUX init...");
    initMux();
    OC_LOG_INFO("[Boot] Phase 3: MUX OK");

    OC_LOG_INFO("[Boot] Phase 4: App init...");
    initApp();
    OC_LOG_INFO("[Boot] Phase 4: App OK");

    OC_LOG_INFO("========================================");
    OC_LOG_INFO("[Boot] COMPLETE - System Ready");
    OC_LOG_INFO("========================================");
}

// Timing constants for main loop
constexpr uint32_t APP_PERIOD_US = 1'000'000 / Config::Timing::APP_HZ;
constexpr uint32_t LVGL_PERIOD_US = 1'000'000 / Config::Timing::LVGL_HZ;

void loop() {
    static uint32_t lastMicros = 0;
    static uint32_t lvglAccumulator = 0;
    static uint32_t loopCount = 0;
    static uint32_t lastHeartbeat = 0;
    static uint32_t initialFreeRAM = 0;

    const uint32_t now = micros();
    if (now - lastMicros < APP_PERIOD_US) return;
    lastMicros = now;

    loopCount++;

    // Initialize on first loop
    if (initialFreeRAM == 0) {
        initialFreeRAM = getFreeRAM();
        lastHeartbeat = millis();
        OC_LOG_INFO("[Loop] Initial free RAM: {} bytes", initialFreeRAM);
    }

    // Heartbeat every 5 seconds
    const uint32_t nowMs = millis();
    if (nowMs - lastHeartbeat >= 5000) {
        lastHeartbeat = nowMs;
        uint32_t currentFreeRAM = getFreeRAM();
        int32_t ramDelta = static_cast<int32_t>(currentFreeRAM) - static_cast<int32_t>(initialFreeRAM);
        OC_LOG_INFO("[Heartbeat] loops={}, uptime={}s, freeRAM={} (delta={})",
                    loopCount, nowMs / 1000, currentFreeRAM, ramDelta);
    }

    // Poll hardware and update active context
    app->update();

    // Refresh LVGL at lower frequency to reduce CPU load
    lvglAccumulator += APP_PERIOD_US;
    if (lvglAccumulator >= LVGL_PERIOD_US) {
        lvglAccumulator = 0;
        lvgl->refresh();
    }
}
