#pragma once

/**
 * Encoder.hpp
 *
 * AUTO-GENERATED - DO NOT EDIT
 */

// Serial8 Encoder - 8-bit binary

#include <cstdint>
#include <cstring>
#include <string>

namespace Protocol {

struct Encoder {

/**
 * Encode bool (1 byte)
 * Boolean value (true/false) (0x00 or 0x01)
 */
static inline void encodeBool(uint8_t*& buf, bool val) {
    *buf++ = val ? 0x01 : 0x00;
}

/**
 * Encode float32 (4 bytes)
 * 32-bit IEEE 754 floating point (4 bytes, IEEE 754 little-endian)
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
 * Encode int16 (2 bytes)
 * 16-bit signed integer (-32768 to 32767) (2 bytes, little-endian)
 */
static inline void encodeInt16(uint8_t*& buf, int16_t value) {
    uint16_t val = static_cast<uint16_t>(value);
    *buf++ = val & 0xFF;
    *buf++ = (val >> 8) & 0xFF;
}

/**
 * Encode int32 (4 bytes)
 * 32-bit signed integer (-2147483648 to 2147483647) (4 bytes, little-endian)
 */
static inline void encodeInt32(uint8_t*& buf, int32_t value) {
    uint32_t val = static_cast<uint32_t>(value);
    *buf++ = val & 0xFF;
    *buf++ = (val >> 8) & 0xFF;
    *buf++ = (val >> 16) & 0xFF;
    *buf++ = (val >> 24) & 0xFF;
}

/**
 * Encode int8 (1 byte)
 * 8-bit signed integer (-128 to 127) (1 byte, direct)
 */
static inline void encodeInt8(uint8_t*& buf, int8_t val) {
    *buf++ = val & 0xFF;
}

/**
 * Encode norm16 (2 bytes)
 * Normalized float (0.0-1.0) stored as uint16 for efficiency (2 bytes, little-endian (0-65535))
 */
static inline void encodeNorm16(uint8_t*& buf, float val) {
    if (val < 0.0f) val = 0.0f;
    if (val > 1.0f) val = 1.0f;
    uint16_t norm = static_cast<uint16_t>(val * 65535.0f + 0.5f);
    *buf++ = norm & 0xFF;
    *buf++ = (norm >> 8) & 0xFF;
}

/**
 * Encode norm8 (1 byte)
 * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth (1 byte, full 8-bit range (0-255))
 */
static inline void encodeNorm8(uint8_t*& buf, float val) {
    if (val < 0.0f) val = 0.0f;
    if (val > 1.0f) val = 1.0f;
    uint8_t norm = static_cast<uint8_t>(val * 255.0f + 0.5f);
    *buf++ = norm & 0xFF;
}

/**
 * Encode string (variable length)
 * Variable-length UTF-8 string (prefixed with uint8 length) (1 byte length prefix + data (max 255 chars))
 *
 * Format: [length] [char0] [char1] ... [charN-1]
 * Max length: 255 chars
 */
static inline void encodeString(uint8_t*& buf, const std::string& str) {
    uint8_t len = static_cast<uint8_t>(str.length()) & 0xFF;
    *buf++ = len;

    for (size_t i = 0; i < len; ++i) {
        *buf++ = static_cast<uint8_t>(str[i]) & 0xFF;
    }
}

/**
 * Encode uint16 (2 bytes)
 * 16-bit unsigned integer (0-65535) (2 bytes, little-endian)
 */
static inline void encodeUint16(uint8_t*& buf, uint16_t val) {
    *buf++ = val & 0xFF;
    *buf++ = (val >> 8) & 0xFF;
}

/**
 * Encode uint32 (4 bytes)
 * 32-bit unsigned integer (0-4294967295) (4 bytes, little-endian)
 */
static inline void encodeUint32(uint8_t*& buf, uint32_t val) {
    *buf++ = val & 0xFF;
    *buf++ = (val >> 8) & 0xFF;
    *buf++ = (val >> 16) & 0xFF;
    *buf++ = (val >> 24) & 0xFF;
}

/**
 * Encode uint8 (1 byte)
 * 8-bit unsigned integer (0-255) (1 byte, direct)
 */
static inline void encodeUint8(uint8_t*& buf, uint8_t val) {
    *buf++ = val & 0xFF;
}
};

}  // namespace Protocol
