#pragma once

#include <cstdint>

namespace bitwig::config::LastClickedConfig {

inline constexpr uint32_t SEND_INTERVAL_MS = 12;
inline constexpr float KNOB_EPSILON = 0.0025f;
inline constexpr float POSITION_RESYNC_EPSILON = 0.001f;

}  // namespace bitwig::config::LastClickedConfig
