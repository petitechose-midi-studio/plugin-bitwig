#pragma once

/**
 * @file LastClickedInputHandler.hpp
 * @brief Handles last clicked parameter input -> sends protocol messages
 *
 * InputHandler pattern:
 * - OPT encoder turn: adjust value of last clicked parameter in Bitwig
 *
 * The "last clicked" parameter is the most recently clicked parameter in Bitwig's UI.
 * This allows quick adjustment without navigating to a specific device/page.
 */

#include <lvgl.h>

#include <cstdint>
#include <optional>

#include <oc/api/EncoderAPI.hpp>

#include "config/LastClickedConfig.hpp"
#include "protocol/BitwigProtocol.hpp"
#include "state/BitwigState.hpp"

namespace bitwig::handler {

/**
 * @brief Last clicked parameter input handler (Input -> Protocol)
 */
class LastClickedInputHandler {
public:
    LastClickedInputHandler(state::BitwigState& state,
                            BitwigProtocol& protocol,
                            oc::api::EncoderAPI& encoders,
                            lv_obj_t* scopeElement);

    ~LastClickedInputHandler() = default;
    void flushPending();

    // Non-copyable
    LastClickedInputHandler(const LastClickedInputHandler&) = delete;
    LastClickedInputHandler& operator=(const LastClickedInputHandler&) = delete;

private:
    void setupBindings();
    void handleEncoderTurn(float value);

    bool canHandleCurrentParameter() const;
    bool shouldQueueValue(float value) const;
    bool isEquivalentToLastSent(float value) const;
    void resetTransmissionStateIfParameterChanged();
    int toDiscreteIndex(float value) const;
    bool toButtonState(float value) const;

    state::BitwigState& state_;
    BitwigProtocol& protocol_;
    oc::api::EncoderAPI& encoders_;
    lv_obj_t* scope_element_;
    std::optional<float> pending_value_;
    uint32_t last_send_ms_ = 0;
    float last_sent_value_ = -1.0f;
    int last_sent_discrete_index_ = -1;
    int pending_discrete_index_ = -1;
    bool last_sent_button_state_ = false;
    bool has_last_sent_button_state_ = false;
    bool last_parameter_exists_ = false;
    ParameterType last_parameter_type_ = ParameterType::KNOB;
    int16_t last_discrete_count_ = -1;

};

}  // namespace bitwig::handler
