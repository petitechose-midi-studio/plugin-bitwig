/**
 * RequestTrackSendListMessage.hpp - Auto-generated Protocol Struct
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: types.yaml
 *
 * Description: REQUEST_TRACK_SEND_LIST message
 *
 * This struct uses encode/decode functions from Protocol namespace.
 * All encoding is 8-bit binary (Serial8). Performance is identical to inline
 * code due to static inline + compiler optimization.
 */

#pragma once

#include "../Encoder.hpp"
#include "../Decoder.hpp"
#include "../MessageID.hpp"
#include "../ProtocolConstants.hpp"
#include "../Logger.hpp"
#include <cstdint>
#include <optional>

namespace Protocol {



struct RequestTrackSendListMessage {
    // Auto-detected MessageID for protocol.send()
    static constexpr MessageID MESSAGE_ID = MessageID::REQUEST_TRACK_SEND_LIST;


    // Origin tracking (set by DecoderRegistry during decode)
    bool fromHost = false;

    /**
     * Maximum payload size in bytes (8-bit encoded)
     */
    static constexpr uint16_t MAX_PAYLOAD_SIZE = 0;

    /**
     * Minimum payload size in bytes (with empty strings)
     */
    static constexpr uint16_t MIN_PAYLOAD_SIZE = 0;

    /**
     * Encode struct to MIDI-safe bytes (empty message)
     * @return Always 0 (no payload)
     */
    uint16_t encode(uint8_t*, uint16_t) const { return 0; }

    /**
     * Decode struct from MIDI-safe bytes (empty message)
     * @return Always returns empty struct
     */
    static std::optional<RequestTrackSendListMessage> decode(const uint8_t*, uint16_t) {
        return RequestTrackSendListMessage{};
    }


    /**
     * Convert message to YAML format for logging
     *
     * WARNING: Uses shared g_logBuffer - log immediately!
     * Multiple calls will overwrite previous results.
     *
     * @return YAML string representation
     */
    const char* toString() const {
        char* ptr = g_logBuffer;
        const char* end = g_logBuffer + LOG_BUFFER_SIZE - 1;

        ptr += snprintf(ptr, end - ptr, "# RequestTrackSendList\nrequestTrackSendList:\n");


        *ptr = '\0';
        return g_logBuffer;
    }

};

}  // namespace Protocol
