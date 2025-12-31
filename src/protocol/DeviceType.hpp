/**
 * DeviceType.hpp - Auto-generated Protocol Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Description: Type of device in the DAW
 *
 */

#pragma once

#include <cstdint>

enum class DeviceType : uint8_t {
    UNKNOWN = 0,
    AUDIO_EFFECT = 1,
    INSTRUMENT = 2,
    NOTE_EFFECT = 3,
};

// Conversion helpers
inline DeviceType toDeviceType(uint8_t value) {
    return static_cast<DeviceType>(value);
}

inline uint8_t fromDeviceType(DeviceType value) {
    return static_cast<uint8_t>(value);
}
