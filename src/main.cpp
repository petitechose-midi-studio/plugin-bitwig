/**
 * @file main.cpp
 * @brief MIDI Studio - Bitwig Plugin
 *
 * Uses oc::hal::teensy::AppBuilder for Teensy 4.1 setup.
 * Registers BitwigContext as the main (and only) context.
 */

#include <optional>

#include <Arduino.h>
#include <ms/device_support/v1/Buffers.hpp>
#include <ms/device_support/v1/Display.hpp>
#include <ms/device_support/v1/Hardware.hpp>
#include <ms/device_support/v1/InputConfig.hpp>
#include <ms/device_support/v1/Timing.hpp>
#include <oc/hal/teensy/Teensy.hpp>

#include "app/AppLogic.hpp"

namespace device = ms::device_support::v1;

// =============================================================================
// Debug: Memory monitoring for crash diagnosis
// =============================================================================
extern "C" char* sbrk(int incr);

static uint32_t getFreeRAM() {
    char top;
    return &top - sbrk(0);
}



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

static void checkOrHalt(const oc::type::Result<void>& result, const char* component) {
    if (!result) {
        OC_LOG_ERROR("{} init failed: {}", component,
                     oc::type::errorCodeToString(result.error().code));
        while (true) {}
    }
}

static void initDisplay() {
    display = oc::hal::teensy::Ili9341(
        device::display::CONFIG,
        {.framebuffer = device::buffers::framebuffer,
         .diff1 = device::buffers::diff1,
         .diff2 = device::buffers::diff2});
    checkOrHalt(display->init(), "Display");
}

static void initLVGL() {
    lvgl = oc::ui::lvgl::Bridge(*display, device::buffers::lvgl,
                                 oc::hal::teensy::defaultTimeProvider,
                                 device::display::LVGL_CONFIG);
    checkOrHalt(lvgl->init(), "LVGL");
}

static void initMux() {
    mux = oc::hal::teensy::CD74HC4067(device::mux::CONFIG,
                                      oc::hal::teensy::gpio());
    checkOrHalt(mux->init(), "MUX");
}

static void initApp() {
    app = oc::hal::teensy::AppBuilder()
              .midi()
              .frames()
              .encoders(device::encoder::ENCODERS)
              .buttons(device::button::BUTTONS, *mux, device::timing::DEBOUNCE_MS)
              .inputConfig(device::input::CONFIG);

    bitwig::app::registerContexts(*app);
    app->begin();
}

// =============================================================================
// Arduino Entry Points
// =============================================================================

void setup() {
    // NOTE: Logging enabled temporarily for debug - may interfere with protocol
    oc::hal::teensy::initLogging();

    OC_LOG_INFO("MIDI Studio Bitwig Plugin ({}Hz)",
                device::timing::INPUT_APP_ADMISSION_HZ);

    initDisplay();
    initLVGL();
    initMux();
    initApp();
}

// Timing constants for main loop
constexpr uint32_t APP_PERIOD_US =
    1'000'000 / device::timing::INPUT_APP_ADMISSION_HZ;
constexpr uint32_t LVGL_PERIOD_US =
    1'000'000 / device::timing::LVGL_SERVICE_HZ;

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
