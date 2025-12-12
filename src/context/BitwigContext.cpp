#include "BitwigContext.hpp"

#include <Arduino.h>  // For Serial debug
#include <oc/log/Log.hpp>

#include <ui/font/FontLoader.hpp>  // Core's font system
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
    Serial.println("[BitwigContext] initialize() called");
    OC_LOG_INFO("BitwigContext initializing...");

    // Load fonts: core text fonts + plugin icon fonts
    Serial.println("[BitwigContext] Loading fonts...");
    fontsRegisterCore();   // Register core's Inter Display fonts
    fontsLoadEssential();  // Load splash fonts immediately
    loadPluginFonts();     // Load all remaining fonts (core + icon fonts)

    // Link plugin fonts to core's loaded fonts (now safe)
    bitwigFontsLink();
    Serial.println("[BitwigContext] Fonts loaded");

    Serial.println("[BitwigContext] Creating protocol...");
    createProtocol();
    Serial.println("[BitwigContext] Creating host handlers...");
    createHostHandlers();
    Serial.println("[BitwigContext] Creating views...");
    createViews();
    Serial.println("[BitwigContext] Creating input handlers...");
    createInputHandlers();

    Serial.println("[BitwigContext] Initialized OK");
    OC_LOG_INFO("BitwigContext initialized");
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

    // Views
    deviceView_.reset();

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
    protocol_ = std::make_unique<BitwigProtocol>(midi(), events());
}

void BitwigContext::createHostHandlers() {
    // Create host handlers - they set up protocol callbacks in their constructors
    // Order matters: plugin handler requests host status
    hostPlugin_ = std::make_unique<handler::HandlerHostPlugin>(state_, *protocol_);
    hostTransport_ = std::make_unique<handler::HandlerHostTransport>(state_, *protocol_);
    hostDevice_ = std::make_unique<handler::HandlerHostDevice>(state_, *protocol_, encoders());
    hostLastClicked_ = std::make_unique<handler::HandlerHostLastClicked>(state_, *protocol_, encoders());
    hostMidi_ = std::make_unique<handler::HandlerHostMidi>(state_, midi());
}

void BitwigContext::createInputHandlers() {
    // === SCOPE HIERARCHY (priority: overlay > view > global) ===
    // - Global: No scope - lowest priority (Transport)
    // - View: deviceView_->getElement() - middle priority (Macro, open/close bindings)
    // - Overlay: getXxxSelectorElement() - highest priority (contextual bindings)

    lv_obj_t* scopeElement = deviceView_ ? deviceView_->getElement() : lv_screen_active();

    // Get overlay elements for high-priority contextual bindings
    lv_obj_t* deviceSelectorOverlay = deviceView_ ? deviceView_->getDeviceSelectorElement() : nullptr;
    lv_obj_t* pageSelectorOverlay = deviceView_ ? deviceView_->getPageSelectorElement() : nullptr;
    lv_obj_t* trackSelectorOverlay = deviceView_ ? deviceView_->getTrackSelectorElement() : nullptr;

    // Create input handlers - they set up input bindings in their constructors

    // Transport: GLOBAL bindings (no scope) - lowest priority
    inputTransport_ = std::make_unique<handler::HandlerInputTransport>(
        state_, *protocol_, encoders(), buttons());

    // Macro: VIEW-level scope (always active when view visible)
    inputMacro_ = std::make_unique<handler::HandlerInputMacro>(
        state_, *protocol_, encoders(), buttons(), scopeElement);

    // DevicePage: VIEW + OVERLAY scopes
    inputDevicePage_ = std::make_unique<handler::HandlerInputDevicePage>(
        state_, *protocol_, encoders(), buttons(), scopeElement, pageSelectorOverlay);

    // DeviceSelector: VIEW + OVERLAY scopes
    inputDeviceSelector_ = std::make_unique<handler::HandlerInputDeviceSelector>(
        state_, *protocol_, encoders(), buttons(), scopeElement, deviceSelectorOverlay);

    // Track: OVERLAY scope only (all bindings scoped to track selector)
    inputTrack_ = std::make_unique<handler::HandlerInputTrack>(
        state_, *protocol_, encoders(), buttons(), trackSelectorOverlay);

    // LastClicked: VIEW-level scope
    inputLastClicked_ = std::make_unique<handler::HandlerInputLastClicked>(
        state_, *protocol_, encoders(), scopeElement);
}

void BitwigContext::createViews() {
    // Create main device view on screen with state reference
    // DeviceView subscribes to state signals in its constructor
    deviceView_ = std::make_unique<DeviceView>(lv_screen_active(), state_);
    deviceView_->onActivate();
}

}  // namespace bitwig
