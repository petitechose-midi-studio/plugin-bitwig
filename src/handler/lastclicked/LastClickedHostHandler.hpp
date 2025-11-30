#pragma once

#include "api/ControllerAPI.hpp"
#include "LastClickedState.hpp"
#include "../../protocol/Protocol.hpp"
#include "../../protocol/struct/LastClickedUpdateMessage.hpp"
#include "../../protocol/struct/LastClickedValueChangeMessage.hpp"

namespace Bitwig {

/**
 * @brief Host handler for LastClicked parameter
 *
 * Receives protocol messages from host:
 * - LAST_CLICKED_UPDATE: Parameter info when user clicks in Bitwig
 * - LAST_CLICKED_VALUE_CHANGE: Echo with display value after value sent
 *
 * Updates shared state and configures encoder accordingly.
 */
class LastClickedHostHandler {
public:
    LastClickedHostHandler(ControllerAPI& api, Protocol::Protocol& protocol,
                           LastClickedState& state);

private:
    void setupProtocolCallbacks();
    void handleLastClickedUpdate(const Protocol::LastClickedUpdateMessage& msg);
    void handleLastClickedValueChange(const Protocol::LastClickedValueChangeMessage& msg);

    ControllerAPI& api_;
    Protocol::Protocol& protocol_;
    LastClickedState& state_;
};

}  // namespace Bitwig
