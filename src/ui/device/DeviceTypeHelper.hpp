#pragma once

#include <cstdint>

#include "ui/font/icon.hpp"
#include "ui/theme/BitwigTheme.hpp"

namespace bitwig {

/**
 * @brief Helper for device type icons and colors
 *
 * Device types from Bitwig API:
 * - 0 = Unknown/Other
 * - 1 = Audio Effect
 * - 2 = Instrument
 * - 3 = Note Effect
 */
namespace DeviceType {

struct Info {
    const char* icon;
    uint32_t color;
    bool visible;
};

/**
 * @brief Get icon and color for a device type
 * @param deviceType 0=Unknown, 1=Audio, 2=Instrument, 3=Note
 * @return Info struct with icon, color, and visibility
 */
inline Info get(uint8_t deviceType) {
    switch (deviceType) {
        case 1:  // Audio Effect
            return {Icon::DEVICE_AUDIO, bitwig::theme::Color::DEVICE_TYPE_AUDIO, true};
        case 2:  // Instrument
            return {Icon::DEVICE_INSTRUMENT, bitwig::theme::Color::DEVICE_TYPE_INSTRUMENT, true};
        case 3:  // Note Effect
            return {Icon::DEVICE_NOTE, bitwig::theme::Color::DEVICE_TYPE_NOTE, true};
        default:  // Unknown
            return {nullptr, 0, false};
    }
}

}  // namespace DeviceType
}  // namespace bitwig
