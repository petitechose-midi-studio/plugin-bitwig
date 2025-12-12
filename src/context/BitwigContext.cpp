#include "BitwigContext.hpp"

#include <oc/log/Log.hpp>

#include "protocol/MessageStructure.hpp"

// TODO: Include views when implemented
// #include "ui/device/DeviceView.hpp"

namespace bitwig {

using namespace Protocol;

// =============================================================================
// Config IDs (should match core/src/config/App.hpp)
// =============================================================================

namespace Config {

enum class EncoderID : uint16_t {
    MACRO_1 = 301,
    MACRO_2 = 302,
    MACRO_3 = 303,
    MACRO_4 = 304,
    MACRO_5 = 305,
    MACRO_6 = 306,
    MACRO_7 = 307,
    MACRO_8 = 308,
    NAV = 400,
};

enum class ButtonID : uint16_t {
    LEFT_TOP = 10,
    LEFT_CENTER = 11,
    LEFT_BOTTOM = 12,
    BOTTOM_LEFT = 20,
    BOTTOM_CENTER = 21,
    BOTTOM_RIGHT = 22,
    MACRO_1 = 31,
    MACRO_2 = 32,
    MACRO_3 = 33,
    MACRO_4 = 34,
    MACRO_5 = 35,
    MACRO_6 = 36,
    MACRO_7 = 37,
    MACRO_8 = 38,
    NAV = 40,
};

}  // namespace Config

// =============================================================================
// IContext Lifecycle
// =============================================================================

bool BitwigContext::initialize() {
    OC_LOG_INFO("BitwigContext initializing...");

    setupProtocol();
    setupProtocolCallbacks();
    setupInputBindings();
    setupView();

    // Request host status to detect if Bitwig is running
    protocol_->requestHostStatus();

    OC_LOG_INFO("BitwigContext initialized");
    return true;
}

void BitwigContext::update() {
    // Check host timeout (if connected but no ping for too long)
    if (state_.host.connected.get()) {
        // TODO: Implement timeout check using millis()
        // if (millis() - lastHostPing_ > HOST_TIMEOUT_MS) {
        //     handleHostDeactivated();
        // }
    }
}

void BitwigContext::cleanup() {
    OC_LOG_INFO("BitwigContext cleanup");

    view_.reset();
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
// Setup Methods
// =============================================================================

void BitwigContext::setupProtocol() {
    protocol_ = std::make_unique<BitwigProtocol>(midi());
}

void BitwigContext::setupProtocolCallbacks() {
    // Host connection
    protocol_->onHostInitialized = [this](const HostInitializedMessage&) {
        handleHostInitialized();
    };

    protocol_->onHostDeactivated = [this](const HostDeactivatedMessage&) {
        handleHostDeactivated();
    };

    // Transport
    protocol_->onTransportTempo = [this](const TransportTempoMessage& msg) {
        handleTransportTempo(msg.tempo);
    };

    protocol_->onTransportPlay = [this](const TransportPlayMessage& msg) {
        state_.transport.playing.set(msg.playing);
    };

    protocol_->onTransportRecord = [this](const TransportRecordMessage& msg) {
        state_.transport.recording.set(msg.recording);
    };

    // Device
    protocol_->onDeviceChange = [this](const DeviceChangeMessage& msg) {
        handleDeviceChange(msg.deviceName, msg.deviceType, msg.enabled);
    };

    protocol_->onDeviceStateChange = [this](const DeviceStateChangeMessage& msg) {
        state_.device.enabled.set(msg.enabled);
    };

    protocol_->onDevicePageChange = [this](const DevicePageChangeMessage& msg) {
        state_.device.pageName.set(msg.pageName);
        state_.pageSelector.selectedIndex.set(msg.pageIndex);
    };

    // Parameters
    protocol_->onDeviceMacroUpdate = [this](const DeviceMacroUpdateMessage& msg) {
        handleParameterUpdate(msg.index, msg.value, msg.displayValue);
    };

    protocol_->onDeviceMacroNameChange = [this](const DeviceMacroNameChangeMessage& msg) {
        handleParameterName(msg.index, msg.name);
    };

    protocol_->onDeviceMacroValueChange = [this](const DeviceMacroValueChangeMessage& msg) {
        if (msg.index < state::PARAMETER_COUNT) {
            state_.parameters[msg.index].value.set(msg.value);
        }
    };

    // Last clicked parameter
    protocol_->onLastClickedUpdate = [this](const LastClickedUpdateMessage& msg) {
        state_.lastClicked.name.set(msg.name);
        state_.lastClicked.value.set(msg.value);
        state_.lastClicked.displayValue.set(msg.displayValue);
        state_.lastClicked.parameterType.set(
            static_cast<state::ParameterType>(msg.parameterType));
        state_.lastClicked.discreteCount.set(msg.discreteCount);
        state_.lastClicked.exists.set(true);
    };

    // Device/Track lists - TODO: implement when SignalVector is fully tested
    // protocol_->onDeviceList = ...
    // protocol_->onTrackList = ...
}

void BitwigContext::setupInputBindings() {
    using EncID = Config::EncoderID;
    using BtnID = Config::ButtonID;

    // Macro encoders 1-8
    static constexpr std::array<EncID, 8> MACRO_ENCODERS = {
        EncID::MACRO_1, EncID::MACRO_2, EncID::MACRO_3, EncID::MACRO_4,
        EncID::MACRO_5, EncID::MACRO_6, EncID::MACRO_7, EncID::MACRO_8
    };

    static constexpr std::array<BtnID, 8> MACRO_BUTTONS = {
        BtnID::MACRO_1, BtnID::MACRO_2, BtnID::MACRO_3, BtnID::MACRO_4,
        BtnID::MACRO_5, BtnID::MACRO_6, BtnID::MACRO_7, BtnID::MACRO_8
    };

    // Bind macro encoders
    for (uint8_t i = 0; i < 8; ++i) {
        onEncoder(MACRO_ENCODERS[i]).turn().then([this, i](float value) {
            onEncoderTurn(i, value);
        });

        onButton(MACRO_BUTTONS[i]).press().then([this, i] {
            onEncoderPress(i);
        });
    }

    // Transport buttons
    onButton(BtnID::BOTTOM_LEFT).press().then([this] { onPlayPressed(); });
    onButton(BtnID::BOTTOM_CENTER).press().then([this] { onStopPressed(); });
    onButton(BtnID::BOTTOM_RIGHT).press().then([this] { onRecordPressed(); });

    // Navigation encoder (for device/page selection)
    onEncoder(EncID::NAV).turn().then([this](float delta) {
        // TODO: Implement navigation based on current selector state
        (void)delta;
    });
}

void BitwigContext::setupView() {
    // TODO: Create DeviceView when implemented
    // view_ = std::make_unique<DeviceView>(lv_screen_active(), state_);
    // view_->onActivate();
}

// =============================================================================
// Protocol Callback Handlers
// =============================================================================

void BitwigContext::handleHostInitialized() {
    OC_LOG_INFO("Bitwig host connected");
    state_.host.connected.set(true);
    // lastHostPing_ = millis();

    // Request initial state
    protocol_->requestDeviceList();
    protocol_->requestPageNames();
}

void BitwigContext::handleHostDeactivated() {
    OC_LOG_INFO("Bitwig host disconnected");
    state_.host.connected.set(false);
    state_.resetAll();
}

void BitwigContext::handleTransportTempo(float tempo) {
    state_.transport.tempo.set(tempo);
    // lastHostPing_ = millis();
}

void BitwigContext::handleDeviceChange(const std::string& name, uint8_t type, bool enabled) {
    state_.device.name.set(name);
    state_.device.deviceType.set(static_cast<state::DeviceType>(type));
    state_.device.enabled.set(enabled);

    // Reset parameters when device changes
    state_.parameters.resetAll();

    // Request page names for new device
    protocol_->requestPageNames();
}

void BitwigContext::handleParameterUpdate(uint8_t index, float value,
                                          const std::string& displayValue) {
    if (index >= state::PARAMETER_COUNT) return;

    auto& param = state_.parameters[index];
    param.value.set(value);
    param.displayValue.set(displayValue);
    param.visible.set(true);
    param.loading.set(false);
}

void BitwigContext::handleParameterName(uint8_t index, const std::string& name) {
    if (index >= state::PARAMETER_COUNT) return;
    state_.parameters[index].name.set(name);
}

// =============================================================================
// Input Handlers
// =============================================================================

void BitwigContext::onEncoderTurn(uint8_t index, float value) {
    // Update local state (optimistic update)
    state_.parameters[index].value.set(value);

    // Send to Bitwig
    protocol_->sendParameterValue(index, value);
}

void BitwigContext::onEncoderPress(uint8_t index) {
    // Send touch event to Bitwig
    protocol_->sendParameterTouch(index, true);

    // TODO: Handle parameter editing mode (for discrete parameters)
}

void BitwigContext::onPlayPressed() {
    protocol_->togglePlay();
}

void BitwigContext::onRecordPressed() {
    protocol_->toggleRecord();
}

void BitwigContext::onStopPressed() {
    protocol_->stop();
}

}  // namespace bitwig
