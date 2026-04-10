#include "LastClickedInputHandler.hpp"

#include <algorithm>
#include <cmath>

#include <oc/ui/lvgl/Scope.hpp>

#include <config/App.hpp>

namespace bitwig::handler {

using namespace oc::ui::lvgl;
using EncoderID = Config::EncoderID;

LastClickedInputHandler::LastClickedInputHandler(state::BitwigState& state,
                                                 BitwigProtocol& protocol,
                                                 oc::api::EncoderAPI& encoders,
                                                 lv_obj_t* scopeElement)
    : state_(state)
    , protocol_(protocol)
    , encoders_(encoders)
    , scope_element_(scopeElement) {
    setupBindings();
}

void LastClickedInputHandler::setupBindings() {
    // Scoped binding to deviceView
    encoders_.encoder(EncoderID::OPT).turn().scope(scope(scope_element_)).then([this](float value) {
        handleEncoderTurn(value);
    });
}

void LastClickedInputHandler::handleEncoderTurn(float value) {
    resetTransmissionStateIfParameterChanged();

    if (!canHandleCurrentParameter()) return;

    value = std::clamp(value, 0.0f, 1.0f);

    // Optimistic update to keep local UI responsive.
    state_.lastClicked.value.set(value);

    if (!shouldQueueValue(value)) return;

    pending_value_ = value;
    if (state_.lastClicked.parameterType.get() == ParameterType::LIST) {
        pending_discrete_index_ = toDiscreteIndex(value);
    }
}

void LastClickedInputHandler::flushPending() {
    resetTransmissionStateIfParameterChanged();

    if (!pending_value_.has_value()) return;
    if (!canHandleCurrentParameter()) {
        pending_value_.reset();
        pending_discrete_index_ = -1;
        return;
    }

    const uint32_t now = lv_tick_get();
    if (now - last_send_ms_ < config::LastClickedConfig::SEND_INTERVAL_MS) return;

    const float value = *pending_value_;
    if (isEquivalentToLastSent(value)) {
        pending_value_.reset();
        pending_discrete_index_ = -1;
        return;
    }

    protocol_.lastClickedValue(value);

    last_send_ms_ = now;
    last_sent_value_ = value;
    switch (state_.lastClicked.parameterType.get()) {
        case ParameterType::LIST:
            last_sent_discrete_index_ = pending_discrete_index_;
            break;
        case ParameterType::BUTTON:
            last_sent_button_state_ = toButtonState(value);
            has_last_sent_button_state_ = true;
            break;
        case ParameterType::KNOB:
        default:
            break;
    }

    pending_value_.reset();
    pending_discrete_index_ = -1;
}

bool LastClickedInputHandler::canHandleCurrentParameter() const {
    return state_.lastClicked.exists.get();
}

bool LastClickedInputHandler::shouldQueueValue(float value) const {
    switch (state_.lastClicked.parameterType.get()) {
        case ParameterType::LIST:
            return toDiscreteIndex(value) != last_sent_discrete_index_;
        case ParameterType::BUTTON: {
            const bool buttonState = toButtonState(value);
            return !has_last_sent_button_state_ || buttonState != last_sent_button_state_;
        }
        case ParameterType::KNOB:
        default:
            return !isEquivalentToLastSent(value);
    }
}

bool LastClickedInputHandler::isEquivalentToLastSent(float value) const {
    switch (state_.lastClicked.parameterType.get()) {
        case ParameterType::LIST:
            return toDiscreteIndex(value) == last_sent_discrete_index_;
        case ParameterType::BUTTON: {
            if (!has_last_sent_button_state_) return false;
            return toButtonState(value) == last_sent_button_state_;
        }
        case ParameterType::KNOB:
        default:
            return std::abs(value - last_sent_value_) < config::LastClickedConfig::KNOB_EPSILON;
    }
}

void LastClickedInputHandler::resetTransmissionStateIfParameterChanged() {
    const bool exists = state_.lastClicked.exists.get();
    const ParameterType parameterType = state_.lastClicked.parameterType.get();
    const int16_t discreteCount = state_.lastClicked.discreteCount.get();

    if (exists == last_parameter_exists_ &&
        parameterType == last_parameter_type_ &&
        discreteCount == last_discrete_count_) {
        return;
    }

    pending_value_.reset();
    pending_discrete_index_ = -1;
    last_send_ms_ = 0;
    last_sent_value_ = -1.0f;
    last_sent_discrete_index_ = -1;
    last_sent_button_state_ = false;
    has_last_sent_button_state_ = false;

    last_parameter_exists_ = exists;
    last_parameter_type_ = parameterType;
    last_discrete_count_ = discreteCount;
}

int LastClickedInputHandler::toDiscreteIndex(float value) const {
    const int16_t discreteCount = state_.lastClicked.discreteCount.get();
    if (discreteCount <= 1) return 0;

    const float clamped = std::clamp(value, 0.0f, 1.0f);
    const int maxIndex = discreteCount - 1;
    return std::clamp(static_cast<int>(std::round(clamped * static_cast<float>(maxIndex))), 0, maxIndex);
}

bool LastClickedInputHandler::toButtonState(float value) const {
    return value >= 0.5f;
}

}  // namespace bitwig::handler
