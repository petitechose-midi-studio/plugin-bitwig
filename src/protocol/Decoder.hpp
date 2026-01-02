#pragma once

/**
 * Decoder.hpp
 *
 * AUTO-GENERATED - DO NOT EDIT
 */

// Serial8 Decoder - 8-bit binary

#include <cstdint>
#include <cstring>
#include <string>
#include <cstddef>

namespace Protocol {

struct Decoder {

/**
 * Decode bool (1 byte)
 * Boolean value (true/false) (1 byte)
 */
static inline bool decodeBool(
    const uint8_t*& buf, size_t& remaining, bool& out) {
    if (remaining < 1) return false;
    
    out = (*buf++) != 0x00;
    remaining -= 1;
    return true;
}

/**
 * Decode float32 (4 bytes)
 * 32-bit IEEE 754 floating point (4 bytes, IEEE 754 little-endian)
 */
static inline bool decodeFloat32(
    const uint8_t*& buf, size_t& remaining, float& out) {
    if (remaining < 4) return false;
    
    uint32_t bits = buf[0]
        | (static_cast<uint32_t>(buf[1]) << 8)
        | (static_cast<uint32_t>(buf[2]) << 16)
        | (static_cast<uint32_t>(buf[3]) << 24);
    memcpy(&out, &bits, sizeof(float));
    buf += 4;
    remaining -= 4;
    return true;
}

/**
 * Decode int16 (2 bytes)
 * 16-bit signed integer (-32768 to 32767) (2 bytes, little-endian)
 */
static inline bool decodeInt16(
    const uint8_t*& buf, size_t& remaining, int16_t& out) {
    if (remaining < 2) return false;
    
    uint16_t bits = buf[0]
        | (static_cast<uint16_t>(buf[1]) << 8);
    out = static_cast<int16_t>(bits);
    buf += 2;
    remaining -= 2;
    return true;
}

/**
 * Decode int32 (4 bytes)
 * 32-bit signed integer (-2147483648 to 2147483647) (4 bytes, little-endian)
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
 * Decode int8 (1 byte)
 * 8-bit signed integer (-128 to 127) (1 byte, direct)
 */
static inline bool decodeInt8(
    const uint8_t*& buf, size_t& remaining, int8_t& out) {
    if (remaining < 1) return false;
    
    out = buf[0];
    buf += 1;
    remaining -= 1;
    return true;
}

/**
 * Decode norm16 (2 bytes)
 * Normalized float (0.0-1.0) stored as uint16 for efficiency (2 bytes, little-endian (0-65535))
 */
static inline bool decodeNorm16(
    const uint8_t*& buf, size_t& remaining, float& out) {
    if (remaining < 2) return false;
    
    uint16_t raw = buf[0]
        | (static_cast<uint16_t>(buf[1]) << 8);
    buf += 2;
    out = static_cast<float>(raw) / 65535.0f;
    remaining -= 2;
    return true;
}

/**
 * Decode norm8 (1 byte)
 * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth (1 byte, full 8-bit range (0-255))
 */
static inline bool decodeNorm8(
    const uint8_t*& buf, size_t& remaining, float& out) {
    if (remaining < 1) return false;
    
    uint8_t raw = *buf++;
    out = static_cast<float>(raw) / 255.0f;
    remaining -= 1;
    return true;
}

/**
 * Decode string (variable length)
 * Variable-length UTF-8 string (prefixed with uint8 length) (1 byte length prefix + data (max 255 chars))
 *
 * Format: [length] [char0] [char1] ... [charN-1]
 * Max length: 255 chars
 */
static inline bool decodeString(
    const uint8_t*& buf, size_t& remaining, std::string& out) {

    if (remaining < 1) return false;

    uint8_t len = *buf++ & 0xFF;
    remaining -= 1;

    if (remaining < len) return false;

    out.clear();
    out.reserve(len);
    for (uint8_t i = 0; i < len; ++i) {
        out.push_back(static_cast<char>(*buf++ & 0xFF));
    }
    remaining -= len;
    return true;
}

/**
 * Decode uint16 (2 bytes)
 * 16-bit unsigned integer (0-65535) (2 bytes, little-endian)
 */
static inline bool decodeUint16(
    const uint8_t*& buf, size_t& remaining, uint16_t& out) {
    if (remaining < 2) return false;
    
    out = buf[0]
        | (static_cast<uint16_t>(buf[1]) << 8);
    buf += 2;
    remaining -= 2;
    return true;
}

/**
 * Decode uint32 (4 bytes)
 * 32-bit unsigned integer (0-4294967295) (4 bytes, little-endian)
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
 * Decode uint8 (1 byte)
 * 8-bit unsigned integer (0-255) (1 byte, direct)
 */
static inline bool decodeUint8(
    const uint8_t*& buf, size_t& remaining, uint8_t& out) {
    if (remaining < 1) return false;
    
    out = buf[0];
    buf += 1;
    remaining -= 1;
    return true;
}
};

}  // namespace Protocol
