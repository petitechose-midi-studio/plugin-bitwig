#include "BitwigContext.hpp"

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
    OC_LOG_INFO("BitwigContext initializing...");

    // Load fonts: core text fonts + plugin icon fonts
    fontsRegisterCore();
    fontsLoadEssential();
    loadPluginFonts();
    bitwigFontsLink();

    createProtocol();
    createHostHandlers();
    createViews();
    createInputHandlers();

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
    protocol_ = std::make_unique<BitwigProtocol>(midi(), events());
}

void BitwigContext::createHostHandlers() {
    // Order matters: plugin handler requests host status first
    hostPlugin_ = std::make_unique<handler::HandlerHostPlugin>(state_, *protocol_);
    hostTransport_ = std::make_unique<handler::HandlerHostTransport>(state_, *protocol_);
    hostDevice_ = std::make_unique<handler::HandlerHostDevice>(state_, *protocol_, encoders());
    hostLastClicked_ = std::make_unique<handler::HandlerHostLastClicked>(state_, *protocol_, encoders());
    hostMidi_ = std::make_unique<handler::HandlerHostMidi>(state_, midi());
}

void BitwigContext::createInputHandlers() {
    // Scope hierarchy: overlay > view > global
    lv_obj_t* scopeElement = deviceView_ ? deviceView_->getElement() : lv_screen_active();
    lv_obj_t* deviceSelectorOverlay = deviceView_ ? deviceView_->getDeviceSelectorElement() : nullptr;
    lv_obj_t* trackSelectorOverlay = deviceView_ ? deviceView_->getTrackSelectorElement() : nullptr;
    lv_obj_t* pageSelectorOverlay = deviceView_ ? deviceView_->getPageSelectorElement() : nullptr;

    // Global scope
    inputTransport_ = std::make_unique<handler::HandlerInputTransport>(
        state_, *protocol_, encoders(), buttons());

    // View + overlay scopes
    inputDeviceSelector_ = std::make_unique<handler::HandlerInputDeviceSelector>(
        state_, *protocol_, encoders(), buttons(), scopeElement, deviceSelectorOverlay);
    inputMacro_ = std::make_unique<handler::HandlerInputMacro>(
        state_, *protocol_, encoders(), buttons(), scopeElement);
    inputTrack_ = std::make_unique<handler::HandlerInputTrack>(
        state_, *protocol_, encoders(), buttons(), trackSelectorOverlay);
    inputDevicePage_ = std::make_unique<handler::HandlerInputDevicePage>(
        state_, *protocol_, encoders(), buttons(), scopeElement, pageSelectorOverlay);
}

void BitwigContext::createViews() {
    viewContainer_ = std::make_unique<ViewContainer>(lv_screen_active());
    deviceView_ = std::make_unique<DeviceView>(viewContainer_->getMainZone(), state_);
    deviceView_->onActivate();
    transportBar_ = std::make_unique<TransportBar>(viewContainer_->getBottomZone(), state_.transport);
    lv_obj_clear_flag(viewContainer_->getContainer(), LV_OBJ_FLAG_HIDDEN);
}

}  // namespace bitwig
