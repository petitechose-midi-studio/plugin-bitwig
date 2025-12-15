#include "BitwigContext.hpp"

#include <Arduino.h>  // For Serial debug
#include <oc/log/Log.hpp>
#include <oc/teensy/LogOutput.hpp>  // Must be before Log.hpp for OC_LOG_PRINT
#include <ui/font/FontLoader.hpp>   // Core's font system

#include "ui/font/BitwigFonts.hpp"

namespace bitwig {

// =============================================================================
// Static Resource Loading (called by ContextManager during registration)
// =============================================================================

void BitwigContext::loadResources() {
    OC_LOG_INFO("BitwigContext::loadResources()");
    bitwigFontsRegister();  // Queue icon fonts for loading
}

// Destructor must be in .cpp where handler types are complete
BitwigContext::~BitwigContext() = default;

// =============================================================================
// IContext Lifecycle
// =============================================================================

bool BitwigContext::initialize() {
    // DEBUG: Direct serial output to diagnose OC_LOG issue
    Serial.println(">>> BitwigContext::initialize() ENTRY <<<");
    Serial.flush();

    OC_LOG_INFO("========================================");
    OC_LOG_INFO("[Context] BitwigContext::initialize() START");
    OC_LOG_INFO("========================================");

    // Load fonts: core text fonts + plugin icon fonts
    OC_LOG_INFO("[Context] Step 1: Loading fonts...");
    OC_LOG_INFO("[Context]   1.1: fontsRegisterCore()");
    fontsRegisterCore();   // Register core's Inter Display fonts
    OC_LOG_INFO("[Context]   1.2: fontsLoadEssential()");
    fontsLoadEssential();  // Load splash fonts immediately
    OC_LOG_INFO("[Context]   1.3: loadPluginFonts()");
    loadPluginFonts();     // Load all remaining fonts (core + icon fonts)

    // Link plugin fonts to core's loaded fonts (now safe)
    OC_LOG_INFO("[Context]   1.4: bitwigFontsLink()");
    bitwigFontsLink();
    OC_LOG_INFO("[Context] Step 1: Fonts DONE");

    OC_LOG_INFO("[Context] Step 2: createProtocol()...");
    createProtocol();
    OC_LOG_INFO("[Context] Step 2: Protocol DONE");

    OC_LOG_INFO("[Context] Step 3: createHostHandlers()...");
    createHostHandlers();
    OC_LOG_INFO("[Context] Step 3: Host handlers DONE");

    OC_LOG_INFO("[Context] Step 4: createViews()...");
    createViews();
    OC_LOG_INFO("[Context] Step 4: Views DONE");

    OC_LOG_INFO("[Context] Step 5: createInputHandlers()...");
    createInputHandlers();
    OC_LOG_INFO("[Context] Step 5: Input handlers DONE");

    OC_LOG_INFO("========================================");
    OC_LOG_INFO("[Context] BitwigContext::initialize() COMPLETE");
    OC_LOG_INFO("========================================");
    return true;
}

void BitwigContext::update() {
    // Handlers and views are self-updating via Signal subscriptions
    // Only need to check host timeout here if implemented
}

void BitwigContext::cleanup() {
    OC_LOG_INFO("BitwigContext cleanup");

    // Destroy in reverse order of creation

    // Input Handlers (they hold bindings that may reference state)
    inputLastClicked_.reset();
    inputTrack_.reset();
    inputDeviceSelector_.reset();
    inputDevicePage_.reset();
    inputMacro_.reset();
    inputTransport_.reset();

    transportBar_.reset();
    deviceView_.reset();
    viewContainer_.reset();

    // Host Handlers
    hostMidi_.reset();
    hostLastClicked_.reset();
    hostDevice_.reset();
    hostTransport_.reset();
    hostPlugin_.reset();

    // Protocol
    protocol_.reset();

    // State
    state_.resetAll();
}

void BitwigContext::onConnected() {
    OC_LOG_INFO("BitwigContext activated");

    // Request current state from Bitwig
    protocol_->requestHostStatus();
    protocol_->requestDeviceList();
    protocol_->requestTrackList();
}

void BitwigContext::onDisconnected() {
    OC_LOG_INFO("BitwigContext deactivated");
    state_.resetAll();
}

// =============================================================================
// Factory Methods
// =============================================================================

void BitwigContext::createProtocol() {
    OC_LOG_INFO("[Context]   2.1: Creating BitwigProtocol instance...");
    protocol_ = std::make_unique<BitwigProtocol>(midi(), events());
    OC_LOG_INFO("[Context]   2.2: BitwigProtocol created (EventBus subscription active)");
}

void BitwigContext::createHostHandlers() {
    // Create host handlers - they set up protocol callbacks in their constructors
    // Order matters: plugin handler requests host status
    OC_LOG_INFO("[Context]   3.1: Creating HandlerHostPlugin...");
    hostPlugin_ = std::make_unique<handler::HandlerHostPlugin>(state_, *protocol_);

    OC_LOG_INFO("[Context]   3.2: Creating HandlerHostTransport...");
    hostTransport_ = std::make_unique<handler::HandlerHostTransport>(state_, *protocol_);

    OC_LOG_INFO("[Context]   3.3: Creating HandlerHostDevice...");
    hostDevice_ = std::make_unique<handler::HandlerHostDevice>(state_, *protocol_, encoders());

    OC_LOG_INFO("[Context]   3.4: Creating HandlerHostLastClicked...");
    hostLastClicked_ = std::make_unique<handler::HandlerHostLastClicked>(state_, *protocol_, encoders());

    OC_LOG_INFO("[Context]   3.5: Creating HandlerHostMidi...");
    hostMidi_ = std::make_unique<handler::HandlerHostMidi>(state_, midi());
}

void BitwigContext::createInputHandlers() {
    // === SCOPE HIERARCHY (priority: overlay > view > global) ===
    // - Global: No scope - lowest priority (Transport)
    // - View: deviceView_->getElement() - middle priority (Macro, open/close bindings)
    // - Overlay: getXxxSelectorElement() - highest priority (contextual bindings)

    lv_obj_t* scopeElement = deviceView_ ? deviceView_->getElement() : lv_screen_active();
    OC_LOG_INFO("[Context]   5.0: scopeElement obtained (deviceView_={})",
                deviceView_ ? "valid" : "null");

    // Get overlay elements for high-priority contextual bindings
    lv_obj_t* deviceSelectorOverlay = deviceView_ ? deviceView_->getDeviceSelectorElement() : nullptr;
    lv_obj_t* pageSelectorOverlay = deviceView_ ? deviceView_->getPageSelectorElement() : nullptr;
    lv_obj_t* trackSelectorOverlay = deviceView_ ? deviceView_->getTrackSelectorElement() : nullptr;

    // Create input handlers - they set up input bindings in their constructors

    // Transport: GLOBAL bindings (no scope) - lowest priority
    OC_LOG_INFO("[Context]   5.1: Creating HandlerInputTransport (GLOBAL scope)...");
    inputTransport_ = std::make_unique<handler::HandlerInputTransport>(
        state_, *protocol_, encoders(), buttons());
    OC_LOG_INFO("[Context]   5.1: HandlerInputTransport DONE");

    // ========================================================================
    // PROGRESSIVE RE-ENABLE - Starting with DeviceSelector only
    // ========================================================================

    // DeviceSelector: VIEW + OVERLAY scopes
    OC_LOG_INFO("[Context]   5.2: Creating HandlerInputDeviceSelector (VIEW+OVERLAY)...");
    inputDeviceSelector_ = std::make_unique<handler::HandlerInputDeviceSelector>(
        state_, *protocol_, encoders(), buttons(), scopeElement, deviceSelectorOverlay);
    OC_LOG_INFO("[Context]   5.2: HandlerInputDeviceSelector DONE");

    // Macro: VIEW scope - handles encoder input for 8 macro parameters
    OC_LOG_INFO("[Context]   5.3: Creating HandlerInputMacro (VIEW scope)...");
    inputMacro_ = std::make_unique<handler::HandlerInputMacro>(state_, *protocol_, encoders(),
                                                               buttons(), scopeElement);
    OC_LOG_INFO("[Context]   5.3: HandlerInputMacro DONE");

    // Track: OVERLAY scope - handles track selector navigation
    OC_LOG_INFO("[Context]   5.4: Creating HandlerInputTrack (OVERLAY scope)...");
    inputTrack_ = std::make_unique<handler::HandlerInputTrack>(
        state_, *protocol_, encoders(), buttons(), trackSelectorOverlay);
    OC_LOG_INFO("[Context]   5.4: HandlerInputTrack DONE");

    // DISABLED FOR NOW:
    // inputDevicePage_, inputLastClicked_
    (void)pageSelectorOverlay;
}

void BitwigContext::createViews() {
    // Create ViewContainer - manages main_zone (DeviceView) + bottom_zone (TransportBar)
    OC_LOG_INFO("[Context]   4.1: Creating ViewContainer...");
    viewContainer_ = std::make_unique<ViewContainer>(lv_screen_active());

    // Create DeviceView in main zone
    OC_LOG_INFO("[Context]   4.2: Creating DeviceView in main_zone...");
    deviceView_ = std::make_unique<DeviceView>(viewContainer_->getMainZone(), state_);
    OC_LOG_INFO("[Context]   4.3: DeviceView created, calling onActivate()...");
    deviceView_->onActivate();

    // Create TransportBar in bottom zone (subscribes to state internally)
    OC_LOG_INFO("[Context]   4.4: Creating TransportBar in bottom_zone...");
    transportBar_ =
        std::make_unique<TransportBar>(viewContainer_->getBottomZone(), state_.transport);

    // Show ViewContainer (hidden by default for splash)
    OC_LOG_INFO("[Context]   4.6: Showing ViewContainer...");
    lv_obj_clear_flag(viewContainer_->getContainer(), LV_OBJ_FLAG_HIDDEN);

    OC_LOG_INFO("[Context]   4.7: Views created");
}

}  // namespace bitwig
