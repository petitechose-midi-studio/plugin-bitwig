/**
 * Encoder.hpp - 8-bit Binary Encoder (Serial8 Protocol)
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This file provides static inline encode functions for all builtin
 * primitive types. Uses full 8-bit byte range for efficient encoding.
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, norm8, norm16, string
 *
 * Encoding Strategy:
 * - Direct byte writes (no 7-bit constraint)
 * - Little-endian for multi-byte integers
 * - IEEE 754 for floats (native byte order)
 * - 8-bit length prefix for strings/arrays
 *
 * Performance:
 * - Static inline = zero runtime overhead
 * - Compiler optimizes away function calls
 * - More efficient than 7-bit encoding (no expansion)
 *
 * Companion file: Decoder.hpp
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>

namespace Protocol {

// ============================================================================
// ENCODE FUNCTIONS (Type -> 8-bit binary bytes)
// ============================================================================


/**
 * Encode bool (1 byte: 0x00 or 0x01)
 * Boolean value (true/false)
 */
static inline void encodeBool(uint8_t*& buf, bool val) {
    *buf++ = val ? 0x01 : 0x00;
}


/**
 * Encode float32 (4 bytes, IEEE 754 little-endian)
 * 32-bit IEEE 754 floating point
 */
static inline void encodeFloat32(uint8_t*& buf, float val) {
    uint32_t bits;
    memcpy(&bits, &val, sizeof(float));

    *buf++ = bits & 0xFF;
    *buf++ = (bits >> 8) & 0xFF;
    *buf++ = (bits >> 16) & 0xFF;
    *buf++ = (bits >> 24) & 0xFF;
}


/**
 * Encode int16 (2 bytes, little-endian)
 * 16-bit signed integer (-32768 to 32767)
 */
static inline void encodeInt16(uint8_t*& buf, int16_t val) {
    uint16_t bits = static_cast<uint16_t>(val);
    *buf++ = bits & 0xFF;
    *buf++ = (bits >> 8) & 0xFF;
}


/**
 * Encode int32 (4 bytes, little-endian)
 * 32-bit signed integer (-2147483648 to 2147483647)
 */
static inline void encodeInt32(uint8_t*& buf, int32_t val) {
    uint32_t bits = static_cast<uint32_t>(val);
    *buf++ = bits & 0xFF;
    *buf++ = (bits >> 8) & 0xFF;
    *buf++ = (bits >> 16) & 0xFF;
    *buf++ = (bits >> 24) & 0xFF;
}


/**
 * Encode int8 (1 byte, direct)
 * 8-bit signed integer (-128 to 127)
 */
static inline void encodeInt8(uint8_t*& buf, int8_t val) {
    *buf++ = static_cast<uint8_t>(val);
}


/**
 * Encode norm16 (2 bytes, little-endian)
 * Normalized float (0.0-1.0) stored as uint16 for efficiency
 *
 * Converts float 0.0-1.0 to uint16 0-65535 for high precision.
 * Precision: ~0.0015% (1/65535)
 */
static inline void encodeNorm16(uint8_t*& buf, float val) {
    if (val < 0.0f) val = 0.0f;
    if (val > 1.0f) val = 1.0f;
    uint16_t norm = static_cast<uint16_t>(val * 65535.0f + 0.5f);

    *buf++ = norm & 0xFF;
    *buf++ = (norm >> 8) & 0xFF;
}


/**
 * Encode norm8 (1 byte, full 8-bit range)
 * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth
 *
 * Converts float 0.0-1.0 to 8-bit value 0-255 for optimal precision.
 * Precision: ~0.4% (1/255), better than 7-bit norm8.
 */
static inline void encodeNorm8(uint8_t*& buf, float val) {
    if (val < 0.0f) val = 0.0f;
    if (val > 1.0f) val = 1.0f;
    *buf++ = static_cast<uint8_t>(val * 255.0f + 0.5f);
}


/**
 * Encode string (variable length: 1 byte length + data)
 * Variable-length UTF-8 string (prefixed with uint8 length)
 *
 * Format: [length (8-bit)] [char0] [char1] ... [charN-1]
 * Max length: 255 chars (8-bit length encoding)
 */
static inline void encodeString(uint8_t*& buf, const std::string& str) {
    uint8_t len = static_cast<uint8_t>(str.length());  // Max 255
    *buf++ = len;

    for (size_t i = 0; i < len; ++i) {
        *buf++ = static_cast<uint8_t>(str[i]);
    }
}


/**
 * Encode uint16 (2 bytes, little-endian)
 * 16-bit unsigned integer (0-65535)
 */
static inline void encodeUint16(uint8_t*& buf, uint16_t val) {
    *buf++ = val & 0xFF;         // low byte
    *buf++ = (val >> 8) & 0xFF;  // high byte
}


/**
 * Encode uint32 (4 bytes, little-endian)
 * 32-bit unsigned integer (0-4294967295)
 */
static inline void encodeUint32(uint8_t*& buf, uint32_t val) {
    *buf++ = val & 0xFF;
    *buf++ = (val >> 8) & 0xFF;
    *buf++ = (val >> 16) & 0xFF;
    *buf++ = (val >> 24) & 0xFF;
}


/**
 * Encode uint8 (1 byte, direct)
 * 8-bit unsigned integer (0-255)
 */
static inline void encodeUint8(uint8_t*& buf, uint8_t val) {
    *buf++ = val;
}


}  // namespace Protocol
