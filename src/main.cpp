/**
 * @file main.cpp
 * @brief MIDI Studio - Bitwig Plugin
 *
 * Uses oc::hal::teensy::AppBuilder for Teensy 4.1 setup.
 * Registers BitwigContext as the main (and only) context.
 */

#include <optional>

#include <Arduino.h>
#include <oc/hal/teensy/Teensy.hpp>

#include <config/App.hpp>
#include <config/platform-teensy/Buffer.hpp>
#include <config/platform-teensy/Hardware.hpp>
#include "context/BitwigBootContext.hpp"
#include "context/BitwigContext.hpp"

// =============================================================================
// Debug: Memory monitoring for crash diagnosis
// =============================================================================
extern "C" char* sbrk(int incr);

static uint32_t getFreeRAM() {
    char top;
    return &top - sbrk(0);
}

// =============================================================================
// Context ID for Bitwig
// =============================================================================

enum class BitwigContextID : uint8_t {
    BOOT = 0,
    BITWIG = 1,
};

// =============================================================================
// Static Objects
// =============================================================================

static std::optional<oc::hal::teensy::Ili9341> display;
static std::optional<oc::ui::lvgl::Bridge> lvgl;
static std::optional<oc::hal::teensy::CD74HC4067> mux;
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
    display = oc::hal::teensy::Ili9341(
        Hardware::Display::CONFIG,
        {.framebuffer = Buffer::framebuffer, .diff1 = Buffer::diff1, .diff2 = Buffer::diff2});
    checkOrHalt(display->init(), "Display");
}

static void initLVGL() {
    lvgl = oc::ui::lvgl::Bridge(*display, Buffer::lvgl, oc::hal::teensy::defaultTimeProvider,
                                 Hardware::LVGL::CONFIG);
    checkOrHalt(lvgl->init(), "LVGL");
}

static void initMux() {
    mux = oc::hal::teensy::CD74HC4067(Hardware::Mux::CONFIG, oc::hal::teensy::gpio());
    checkOrHalt(mux->init(), "MUX");
}

static void initApp() {
    app = oc::hal::teensy::AppBuilder()
              .midi()
              .frames()
              .encoders(Hardware::Encoder::ENCODERS)
              .buttons(Hardware::Button::BUTTONS, *mux, Config::Timing::DEBOUNCE_MS)
              .inputConfig(Config::Input::CONFIG);

    // Boot context first (starts automatically as ID 0)
    app->registerContext<bitwig::BitwigBootContext>(BitwigContextID::BOOT, "Boot");
    app->registerContext<bitwig::BitwigContext>(BitwigContextID::BITWIG, "Bitwig");
    app->begin();
}

// =============================================================================
// Arduino Entry Points
// =============================================================================

void setup() {
    // NOTE: Logging enabled temporarily for debug - may interfere with protocol
    oc::hal::teensy::initLogging();

    OC_LOG_INFO("MIDI Studio Bitwig Plugin ({}Hz)", Config::Timing::APP_HZ);

    initDisplay();
    initLVGL();
    initMux();
    initApp();
}

// Timing constants for main loop
constexpr uint32_t APP_PERIOD_US = 1'000'000 / Config::Timing::APP_HZ;
constexpr uint32_t LVGL_PERIOD_US = 1'000'000 / Config::Timing::LVGL_HZ;

void loop() {
    static uint32_t lastMicros = 0;
    static uint32_t lvglAccumulator = 0;
    static uint32_t initialFreeRAM = 0;

    const uint32_t now = micros();
    if (now - lastMicros < APP_PERIOD_US) return;
    lastMicros = now;

    // Initialize on first loop (RAM monitoring disabled with logging)
    if (initialFreeRAM == 0) {
        initialFreeRAM = getFreeRAM();
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
