#pragma once

#include "../../protocol/Protocol.hpp"

namespace Plugin::Bitwig {

class TransportBarController;

/**
 * @brief Listen to transport updates FROM Bitwig host
 *
 * SERVER AUTHORITY for transport state:
 * - Update UI (idempotent with optimistic update)
 * - Sync InputHandler state (for next toggle)
 */
class TransportHostHandler {
public:
    TransportHostHandler(Protocol::Protocol& protocol, TransportBarController& controller);
    ~TransportHostHandler() = default;

private:
    void setupProtocolCallbacks();

    Protocol::Protocol& protocol_;
    TransportBarController& view_controller_;
};

} // namespace Plugin::Bitwig
