/**
 * Decoder.hpp - 7-bit MIDI-safe Decoder
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This file provides static inline decode functions for all builtin
 * primitive types. Converts 7-bit MIDI-safe bytes back to native types.
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, string
 *
 * Decoding Strategy:
 * - SysEx bytes (7-bit) → Native types
 * - Multi-byte integers: 7-bit chunks → full integers
 * - Float32: 5 bytes → 4 bytes (7-bit chunks → IEEE 754)
 * - String: length prefix + ASCII data
 *
 * Performance:
 * - Static inline = zero runtime overhead
 * - Compiler optimizes away function calls
 * - Identical to manual inline code
 *
 * Companion file: Encoder.hpp (Type → SysEx direction)
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <etl/optional.h>
#include <etl/string.h>

namespace Protocol {

// ============================================================================
// DECODE FUNCTIONS (7-bit MIDI-safe bytes → Type)
// ============================================================================
// Returns bool success, writes decoded value to output parameter
// Output parameter pattern minimizes memory footprint (no optional overhead)
// Returns false if insufficient data or invalid encoding


/**
 * Decode bool (1 byte)
 * Boolean value (true/false)
 */
static inline bool decodeBool(
    const uint8_t*& buf, size_t& remaining, bool& out) {

    if (remaining < 1) return false;

    out = (*buf++) != 0x00;
    remaining -= 1;
    return true;
}


/**
 * Decode float32 (5 bytes → 4 bytes)
 * 32-bit IEEE 754 floating point
 */
static inline bool decodeFloat32(
    const uint8_t*& buf, size_t& remaining, float& out) {

    if (remaining < 5) return false;

    uint32_t bits = (buf[0] & 0x7F)
                  | ((buf[1] & 0x7F) << 7)
                  | ((buf[2] & 0x7F) << 14)
                  | ((buf[3] & 0x7F) << 21)
                  | ((buf[4] & 0x0F) << 28);
    buf += 5;
    remaining -= 5;

    memcpy(&out, &bits, sizeof(float));  // Type-punning via memcpy (safe)
    return true;
}


/**
 * Decode int16 (3 bytes → 2 bytes)
 * 16-bit signed integer (-32768 to 32767)
 */
static inline bool decodeInt16(
    const uint8_t*& buf, size_t& remaining, int16_t& out) {

    if (remaining < 3) return false;

    uint16_t bits = (buf[0] & 0x7F)
                  | ((buf[1] & 0x7F) << 7)
                  | ((buf[2] & 0x03) << 14);
    out = static_cast<int16_t>(bits);
    buf += 3;
    remaining -= 3;
    return true;
}


/**
 * Decode int32 (5 bytes → 4 bytes)
 * 32-bit signed integer (-2147483648 to 2147483647)
 */
static inline bool decodeInt32(
    const uint8_t*& buf, size_t& remaining, int32_t& out) {

    if (remaining < 5) return false;

    uint32_t bits = (buf[0] & 0x7F)
                  | ((buf[1] & 0x7F) << 7)
                  | ((buf[2] & 0x7F) << 14)
                  | ((buf[3] & 0x7F) << 21)
                  | ((buf[4] & 0x0F) << 28);
    out = static_cast<int32_t>(bits);
    buf += 5;
    remaining -= 5;
    return true;
}


/**
 * Decode int8 (1 byte)
 * 8-bit signed integer (-128 to 127)
 */
static inline bool decodeInt8(
    const uint8_t*& buf, size_t& remaining, int8_t& out) {

    if (remaining < 1) return false;

    out = static_cast<int8_t>((*buf++) & 0x7F);
    remaining -= 1;
    return true;
}


/**
 * Decode string (variable length)
 * Variable-length UTF-8 string (prefixed with uint8 length, max 16 chars)
 */
template<size_t MAX_SIZE>
static inline bool decodeString(
    const uint8_t*& buf, size_t& remaining, etl::string<MAX_SIZE>& out) {

    if (remaining < 1) return false;

    uint8_t len = (*buf++) & 0x7F;
    remaining -= 1;

    if (remaining < len) return false;
    if (len > MAX_SIZE) return false;  // String too long

    out.clear();
    for (uint8_t i = 0; i < len; ++i) {
        out.push_back(static_cast<char>(buf[i] & 0x7F));
    }

    buf += len;
    remaining -= len;
    return true;
}


/**
 * Decode uint16 (3 bytes → 2 bytes)
 * 16-bit unsigned integer (0-65535)
 */
static inline bool decodeUint16(
    const uint8_t*& buf, size_t& remaining, uint16_t& out) {

    if (remaining < 3) return false;

    out = (buf[0] & 0x7F)
        | ((buf[1] & 0x7F) << 7)
        | ((buf[2] & 0x03) << 14);
    buf += 3;
    remaining -= 3;
    return true;
}


/**
 * Decode uint32 (5 bytes → 4 bytes)
 * 32-bit unsigned integer (0-4294967295)
 */
static inline bool decodeUint32(
    const uint8_t*& buf, size_t& remaining, uint32_t& out) {

    if (remaining < 5) return false;

    out = (buf[0] & 0x7F)
        | ((buf[1] & 0x7F) << 7)
        | ((buf[2] & 0x7F) << 14)
        | ((buf[3] & 0x7F) << 21)
        | ((buf[4] & 0x0F) << 28);
    buf += 5;
    remaining -= 5;
    return true;
}


/**
 * Decode uint8 (1 byte)
 * 8-bit unsigned integer (0-255)
 */
static inline bool decodeUint8(
    const uint8_t*& buf, size_t& remaining, uint8_t& out) {

    if (remaining < 1) return false;

    out = (*buf++) & 0x7F;
    remaining -= 1;
    return true;
}


}  // namespace Protocol
