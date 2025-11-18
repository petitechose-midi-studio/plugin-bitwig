/**
 * Encoder.hpp - 7-bit MIDI-safe Encoder
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This file provides static inline encode functions for all builtin
 * primitive types. Converts native types to 7-bit MIDI-safe bytes.
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, string
 *
 * Encoding Strategy:
 * - Single-byte types (uint8, int8): No encoding (already < 0x80 when valid)
 * - Multi-byte integers: 7-bit per byte (e.g., uint16: 2→3 bytes)
 * - Float32: 4→5 bytes (7-bit chunks)
 * - String: length prefix (7-bit) + ASCII data
 *
 * Performance:
 * - Static inline = zero runtime overhead
 * - Compiler optimizes away function calls
 * - Identical to manual inline code
 *
 * Companion file: Decoder.hpp (SysEx → Type direction)
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <etl/string.h>

namespace Protocol {

// ============================================================================
// ENCODE FUNCTIONS (Type → 7-bit MIDI-safe bytes)
// ============================================================================


/**
 * Encode bool (1 byte: 0x00 or 0x01)
 * Boolean value (true/false)
 */
static inline void encodeBool(uint8_t*& buf, bool val) {
    *buf++ = val ? 0x01 : 0x00;
}


/**
 * Encode float32 (4 bytes → 5 bytes, 7-bit encoding)
 * 32-bit IEEE 754 floating point
 * Overhead: +25% (4→5 bytes)
 *
 * Uses IEEE 754 bit representation, split into 7-bit chunks.
 */
static inline void encodeFloat32(uint8_t*& buf, float val) {
    uint32_t bits;
    memcpy(&bits, &val, sizeof(float));  // Type-punning via memcpy (safe)

    *buf++ = bits & 0x7F;           // bits 0-6
    *buf++ = (bits >> 7) & 0x7F;    // bits 7-13
    *buf++ = (bits >> 14) & 0x7F;   // bits 14-20
    *buf++ = (bits >> 21) & 0x7F;   // bits 21-27
    *buf++ = (bits >> 28) & 0x0F;   // bits 28-31
}


/**
 * Encode int16 (2 bytes → 3 bytes, 7-bit encoding)
 * 16-bit signed integer (-32768 to 32767)
 * Overhead: +50% (2→3 bytes)
 */
static inline void encodeInt16(uint8_t*& buf, int16_t val) {
    uint16_t bits = static_cast<uint16_t>(val);
    *buf++ = bits & 0x7F;
    *buf++ = (bits >> 7) & 0x7F;
    *buf++ = (bits >> 14) & 0x03;
}


/**
 * Encode int32 (4 bytes → 5 bytes, 7-bit encoding)
 * 32-bit signed integer (-2147483648 to 2147483647)
 * Overhead: +25% (4→5 bytes)
 */
static inline void encodeInt32(uint8_t*& buf, int32_t val) {
    uint32_t bits = static_cast<uint32_t>(val);
    *buf++ = bits & 0x7F;
    *buf++ = (bits >> 7) & 0x7F;
    *buf++ = (bits >> 14) & 0x7F;
    *buf++ = (bits >> 21) & 0x7F;
    *buf++ = (bits >> 28) & 0x0F;
}


/**
 * Encode int8 (1 byte, signed → unsigned mapping)
 * 8-bit signed integer (-128 to 127)
 */
static inline void encodeInt8(uint8_t*& buf, int8_t val) {
    *buf++ = static_cast<uint8_t>(val) & 0x7F;
}


/**
 * Encode string (variable length: 1 byte length + data)
 * Variable-length UTF-8 string (prefixed with uint8 length, max 16 chars)
 *
 * Format: [length (7-bit)] [char0] [char1] ... [charN-1]
 * Max length: 127 chars (7-bit length encoding)
 */
template<size_t MAX_SIZE>
static inline void encodeString(uint8_t*& buf, const etl::string<MAX_SIZE>& str) {
    uint8_t len = static_cast<uint8_t>(str.length()) & 0x7F;  // Max 127
    *buf++ = len;

    for (size_t i = 0; i < len; ++i) {
        *buf++ = static_cast<uint8_t>(str[i]) & 0x7F;  // ASCII chars are already < 0x80
    }
}


/**
 * Encode uint16 (2 bytes → 3 bytes, 7-bit encoding)
 * 16-bit unsigned integer (0-65535)
 * Overhead: +50% (2→3 bytes)
 */
static inline void encodeUint16(uint8_t*& buf, uint16_t val) {
    *buf++ = val & 0x7F;           // bits 0-6
    *buf++ = (val >> 7) & 0x7F;    // bits 7-13
    *buf++ = (val >> 14) & 0x03;   // bits 14-15 (only 2 bits needed)
}


/**
 * Encode uint32 (4 bytes → 5 bytes, 7-bit encoding)
 * 32-bit unsigned integer (0-4294967295)
 * Overhead: +25% (4→5 bytes)
 */
static inline void encodeUint32(uint8_t*& buf, uint32_t val) {
    *buf++ = val & 0x7F;           // bits 0-6
    *buf++ = (val >> 7) & 0x7F;    // bits 7-13
    *buf++ = (val >> 14) & 0x7F;   // bits 14-20
    *buf++ = (val >> 21) & 0x7F;   // bits 21-27
    *buf++ = (val >> 28) & 0x0F;   // bits 28-31 (only 4 bits needed)
}


/**
 * Encode uint8 (1 byte, no transformation needed if < 0x80)
 * 8-bit unsigned integer (0-255)
 */
static inline void encodeUint8(uint8_t*& buf, uint8_t val) {
    *buf++ = val & 0x7F;  // Ensure MIDI-safe
}


}  // namespace Protocol
