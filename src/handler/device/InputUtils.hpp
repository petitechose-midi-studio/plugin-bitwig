#pragma once

#include <cstdint>

namespace Bitwig {

/**
 * @brief Common utilities for input handlers
 */
namespace InputUtils {

    /**
     * @brief Wrap index within range [0, modulo) with negative handling
     */
    inline int wrapIndex(int value, int modulo) {
        return ((value % modulo) + modulo) % modulo;
    }

} // namespace InputUtils

} // namespace Bitwig
