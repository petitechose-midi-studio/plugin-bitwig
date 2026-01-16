#pragma once

#include <cstdint>

#include "protocol/DeviceType.hpp"
#include "ui/font/BitwigIcons.hpp"
#include "ui/theme/BitwigTheme.hpp"

namespace bitwig::ui {

namespace icons = bitwig::icons;

/**
 * @brief Helper for device type icons and colors
 */
namespace DeviceTypeHelper {

struct Info {
    const char* icon;
    uint32_t color;
    bool visible;
};

/**
 * @brief Get icon and color for a device type
 * @param deviceType DeviceType enum value
 * @return Info struct with icon, color, and visibility
 */
inline Info get(DeviceType deviceType) {
    switch (deviceType) {
        case DeviceType::AUDIO_EFFECT:
            return {icons::DEVICE_AUDIO, bitwig::theme::color::DEVICE_TYPE_AUDIO, true};
        case DeviceType::INSTRUMENT:
            return {icons::DEVICE_INSTRUMENT, bitwig::theme::color::DEVICE_TYPE_INSTRUMENT, true};
        case DeviceType::NOTE_EFFECT:
            return {icons::DEVICE_NOTE, bitwig::theme::color::DEVICE_TYPE_NOTE, true};
        default:  // UNKNOWN
            return {nullptr, 0, false};
    }
}

}  // namespace DeviceTypeHelper
}  // namespace bitwig::ui
