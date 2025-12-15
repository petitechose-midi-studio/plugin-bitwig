#pragma once

#include <cstdint>

namespace bitwig::utils {

/**
 * @brief Utilities for nested navigation with back button at index 0
 *
 * In nested navigation mode, the UI shows a "back" button at index 0,
 * shifting all actual items by +1. These utilities convert between:
 *
 * - **Display index**: UI-facing index (0 = back button, 1+ = items)
 * - **Raw index**: Data-facing index (0 = first item)
 *
 * ## Usage
 * @code
 * // Handler receives raw index from host, converts for UI
 * int displayIndex = toDisplayIndex(msg.deviceIndex);  // 0 -> 1
 * state_.deviceNames[displayIndex].set(msg.name);
 *
 * // Input handler receives display index from UI, converts to raw
 * if (isBackButton(displayIndex)) {
 *     exitToParent();
 * } else {
 *     int rawIndex = toRawIndex(displayIndex);  // 1 -> 0
 *     selectDevice(rawIndex);
 * }
 * @endcode
 */

/**
 * @brief Convert UI display index to data raw index
 *
 * @param displayIndex UI index (0 = back button when nested)
 * @param isNested Whether navigation shows back button (default: true)
 * @return Raw data index (displayIndex - 1 if nested)
 */
inline int toRawIndex(int displayIndex, bool isNested = true) {
    return isNested ? displayIndex - 1 : displayIndex;
}

/**
 * @brief Convert data raw index to UI display index
 *
 * @param rawIndex Data index (0 = first item)
 * @param isNested Whether navigation shows back button (default: true)
 * @return UI display index (rawIndex + 1 if nested)
 */
inline int toDisplayIndex(int rawIndex, bool isNested = true) {
    return isNested ? rawIndex + 1 : rawIndex;
}

/**
 * @brief Check if display index represents the back button
 *
 * @param displayIndex UI index to check
 * @return true if index 0 (back button position)
 */
inline bool isBackButton(int displayIndex) {
    return displayIndex == 0;
}

}  // namespace bitwig::utils
