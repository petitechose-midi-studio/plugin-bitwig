/**
 * ProtocolConstants.hpp - Protocol Configuration Constants
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: protocol_config.yaml
 *
 * This file contains all protocol constants including SysEx framing,
 * message structure offsets, and encoding limits.
 *
 * All constants are constexpr (compile-time, zero runtime cost).
 */

#pragma once

#include <cstdint>

namespace Protocol {

// ============================================================================
// SYSEX FRAMING CONSTANTS
// ============================================================================

constexpr uint8_t SYSEX_START = 0xf0;  // SysEx start byte
constexpr uint8_t SYSEX_END = 0xf7;    // SysEx end byte

constexpr uint8_t MANUFACTURER_ID = 0x7f;  // MIDI manufacturer ID
constexpr uint8_t DEVICE_ID = 0x01;        // Device identifier

constexpr uint8_t MIN_MESSAGE_LENGTH = 6;  // Minimum valid SysEx message
constexpr uint8_t MESSAGE_TYPE_OFFSET = 3;  // Position of MessageID byte
constexpr uint8_t FROM_HOST_OFFSET = 4;      // Position of fromHost flag
constexpr uint8_t PAYLOAD_OFFSET = 5;      // Start of payload data

// ============================================================================
// ENCODING LIMITS
// ============================================================================

constexpr uint8_t STRING_MAX_LENGTH = 16;  // Max chars per string field
constexpr uint8_t ARRAY_MAX_ITEMS = 32;      // Max items per array field
constexpr uint16_t MAX_PAYLOAD_SIZE = 256;    // Max payload bytes
constexpr uint16_t MAX_MESSAGE_SIZE = 512;    // Max total message bytes


}  // namespace Protocol
