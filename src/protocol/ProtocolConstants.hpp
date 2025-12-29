/**
 * ProtocolConstants.hpp - Protocol Configuration Constants (Serial8)
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: Serial8Config
 *
 * This file contains all protocol constants including message structure
 * offsets and encoding limits for 8-bit binary protocol.
 *
 * Note: Framing (COBS) is handled by the bridge layer, not the codec.
 *
 * All constants are constexpr (compile-time, zero runtime cost).
 */

#pragma once

#include <cstdint>

namespace Protocol {

// ============================================================================
// MESSAGE STRUCTURE CONSTANTS
// ============================================================================

constexpr uint8_t MESSAGE_TYPE_OFFSET = 0;  // Position of MessageID byte
constexpr uint8_t PAYLOAD_OFFSET = 1;          // Start of payload data
constexpr uint8_t MIN_MESSAGE_LENGTH = 1;  // Minimum valid message size

// ============================================================================
// ENCODING LIMITS
// ============================================================================

constexpr uint8_t STRING_MAX_LENGTH = 32;  // Max chars per string (8-bit length)
constexpr uint8_t ARRAY_MAX_ITEMS = 32;    // Max items per array (8-bit count)
constexpr uint16_t MAX_PAYLOAD_SIZE = 10000;  // Max payload bytes
constexpr uint16_t MAX_MESSAGE_SIZE = 16000;  // Max total message bytes


}  // namespace Protocol
