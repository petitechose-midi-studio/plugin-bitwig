#include "BitwigContext.hpp"

#include <oc/log/Log.hpp>

// TODO: Include adapted handlers when ready
// #include "handler/HostHandler.hpp"
// #include "handler/InputHandler.hpp"

namespace bitwig {

// =============================================================================
// IContext Lifecycle
// =============================================================================

bool BitwigContext::initialize() {
    OC_LOG_INFO("BitwigContext initializing...");

    createProtocol();
    createHandlers();
    createViews();

    // Request host status to detect if Bitwig is running
    protocol_->requestHostStatus();

    OC_LOG_INFO("BitwigContext initialized");
    return true;
}

void BitwigContext::update() {
    // Handlers and views are self-updating via Signal subscriptions
    // Only need to check host timeout here if implemented
}

void BitwigContext::cleanup() {
    OC_LOG_INFO("BitwigContext cleanup");

    // Destroy in reverse order
    view_.reset();
    inputHandler_.reset();
    hostHandler_.reset();
    protocol_.reset();

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
    protocol_ = std::make_unique<BitwigProtocol>(midi());
}

void BitwigContext::createHandlers() {
    // TODO: Create adapted handlers
    // hostHandler_ = std::make_unique<handler::HostHandler>(state_, *protocol_);
    // inputHandler_ = std::make_unique<handler::InputHandler>(state_, *protocol_, *this);
    //
    // Handlers will set up their own protocol callbacks and input bindings
    // in their constructors
}

void BitwigContext::createViews() {
    // TODO: Create views when adapted
    // view_ = std::make_unique<DeviceView>(lv_screen_active(), state_);
    // view_->onActivate();
    //
    // Views will subscribe to state signals in their constructors
}

}  // namespace bitwig
