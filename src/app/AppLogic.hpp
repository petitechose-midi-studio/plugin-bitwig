#pragma once

/**
 * @file AppLogic.hpp
 * @brief Bitwig plugin application logic - context registration
 *
 * Shared between Teensy and SDL builds.
 * Encapsulates context IDs and registration logic.
 */

#include <oc/app/OpenControlApp.hpp>

#include "context/BitwigBootContext.hpp"
#include "context/BitwigContext.hpp"

namespace bitwig {

/// Context identifiers for Bitwig plugin
enum class ContextID : uint8_t {
    BOOT = 0,
    BITWIG = 1,
};

namespace app {

/**
 * @brief Register Bitwig plugin contexts
 *
 * Registers:
 * - BitwigBootContext (ID 0): Splash screen, transitions to Bitwig
 * - BitwigContext (ID 1): Main DAW integration context
 *
 * @param app OpenControlApp instance to register contexts with
 */
inline void registerContexts(oc::app::OpenControlApp& app) {
    app.registerContext<BitwigBootContext>(ContextID::BOOT, "Boot");
    app.registerContext<BitwigContext>(ContextID::BITWIG, "Bitwig");
}

}  // namespace app
}  // namespace bitwig
