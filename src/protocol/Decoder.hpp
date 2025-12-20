/**
 * Decoder.hpp - 8-bit Binary Decoder (Serial8 Protocol)
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This file provides static inline decode functions for all builtin
 * primitive types. Decodes 8-bit binary bytes to native types.
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, norm8, norm16, string
 *
 * Decoding Strategy:
 * - Direct byte reads (no 7-bit constraint)
 * - Little-endian for multi-byte integers
 * - IEEE 754 for floats
 * - 8-bit length prefix for strings/arrays
 *
 * Performance:
 * - Static inline = zero runtime overhead
 * - Compiler optimizes away function calls
 * - More efficient than 7-bit decoding
 *
 * Companion file: Encoder.hpp
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>

namespace Protocol {

// ============================================================================
// DECODE FUNCTIONS (8-bit binary bytes -> Type)
// ============================================================================
// Returns bool success, writes decoded value to output parameter
// Output parameter pattern minimizes memory footprint
// Returns false if insufficient data


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
 * Decode float32 (4 bytes, IEEE 754 little-endian)
 * 32-bit IEEE 754 floating point
 */
static inline bool decodeFloat32(
    const uint8_t*& buf, size_t& remaining, float& out) {

    if (remaining < 4) return false;

    uint32_t bits = buf[0]
                  | (static_cast<uint32_t>(buf[1]) << 8)
                  | (static_cast<uint32_t>(buf[2]) << 16)
                  | (static_cast<uint32_t>(buf[3]) << 24);
    buf += 4;
    remaining -= 4;

    memcpy(&out, &bits, sizeof(float));
    return true;
}


/**
 * Decode int16 (2 bytes, little-endian)
 * 16-bit signed integer (-32768 to 32767)
 */
static inline bool decodeInt16(
    const uint8_t*& buf, size_t& remaining, int16_t& out) {

    if (remaining < 2) return false;

    uint16_t bits = buf[0] | (static_cast<uint16_t>(buf[1]) << 8);
    out = static_cast<int16_t>(bits);
    buf += 2;
    remaining -= 2;
    return true;
}


/**
 * Decode int32 (4 bytes, little-endian)
 * 32-bit signed integer (-2147483648 to 2147483647)
 */
static inline bool decodeInt32(
    const uint8_t*& buf, size_t& remaining, int32_t& out) {

    if (remaining < 4) return false;

    uint32_t bits = buf[0]
                  | (static_cast<uint32_t>(buf[1]) << 8)
                  | (static_cast<uint32_t>(buf[2]) << 16)
                  | (static_cast<uint32_t>(buf[3]) << 24);
    out = static_cast<int32_t>(bits);
    buf += 4;
    remaining -= 4;
    return true;
}


/**
 * Decode int8 (1 byte, direct)
 * 8-bit signed integer (-128 to 127)
 */
static inline bool decodeInt8(
    const uint8_t*& buf, size_t& remaining, int8_t& out) {

    if (remaining < 1) return false;

    out = static_cast<int8_t>(*buf++);
    remaining -= 1;
    return true;
}


/**
 * Decode norm16 (2 bytes -> float 0.0-1.0)
 * Normalized float (0.0-1.0) stored as uint16 for efficiency
 */
static inline bool decodeNorm16(
    const uint8_t*& buf, size_t& remaining, float& out) {

    if (remaining < 2) return false;

    uint16_t val = buf[0] | (static_cast<uint16_t>(buf[1]) << 8);
    buf += 2;
    remaining -= 2;

    out = static_cast<float>(val) / 65535.0f;
    return true;
}


/**
 * Decode norm8 (1 byte -> float 0.0-1.0)
 * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth
 */
static inline bool decodeNorm8(
    const uint8_t*& buf, size_t& remaining, float& out) {

    if (remaining < 1) return false;

    uint8_t val = *buf++;
    remaining -= 1;

    out = static_cast<float>(val) / 255.0f;
    return true;
}


/**
 * Decode string (variable length)
 * Variable-length UTF-8 string (prefixed with uint8 length)
 */
static inline bool decodeString(
    const uint8_t*& buf, size_t& remaining, std::string& out) {

    if (remaining < 1) return false;

    uint8_t len = *buf++;
    remaining -= 1;

    if (remaining < len) return false;

    out.assign(reinterpret_cast<const char*>(buf), len);
    buf += len;
    remaining -= len;
    return true;
}


/**
 * Decode uint16 (2 bytes, little-endian)
 * 16-bit unsigned integer (0-65535)
 */
static inline bool decodeUint16(
    const uint8_t*& buf, size_t& remaining, uint16_t& out) {

    if (remaining < 2) return false;

    out = buf[0] | (static_cast<uint16_t>(buf[1]) << 8);
    buf += 2;
    remaining -= 2;
    return true;
}


/**
 * Decode uint32 (4 bytes, little-endian)
 * 32-bit unsigned integer (0-4294967295)
 */
static inline bool decodeUint32(
    const uint8_t*& buf, size_t& remaining, uint32_t& out) {

    if (remaining < 4) return false;

    out = buf[0]
        | (static_cast<uint32_t>(buf[1]) << 8)
        | (static_cast<uint32_t>(buf[2]) << 16)
        | (static_cast<uint32_t>(buf[3]) << 24);
    buf += 4;
    remaining -= 4;
    return true;
}


/**
 * Decode uint8 (1 byte, direct)
 * 8-bit unsigned integer (0-255)
 */
static inline bool decodeUint8(
    const uint8_t*& buf, size_t& remaining, uint8_t& out) {

    if (remaining < 1) return false;

    out = *buf++;
    remaining -= 1;
    return true;
}


}  // namespace Protocol
