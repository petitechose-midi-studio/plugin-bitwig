/**
 * TrackType.hpp - Auto-generated Protocol Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Description: Type of track in the DAW
 *
 */

#pragma once

#include <cstdint>

enum class TrackType : uint8_t {
    AUDIO = 0,
    INSTRUMENT = 1,
    HYBRID = 2,
    GROUP = 3,
    EFFECT = 4,
    MASTER = 5,
    COUNT = 6,  // Sentinel - must be last
};

// Conversion helpers
inline TrackType toTrackType(uint8_t value) {
    return static_cast<TrackType>(value);
}

inline uint8_t fromTrackType(TrackType value) {
    return static_cast<uint8_t>(value);
}

// Name helper
inline const char* trackTypeName(TrackType value) {
    switch (value) {
        case TrackType::AUDIO: return "Audio";
        case TrackType::INSTRUMENT: return "Instrument";
        case TrackType::HYBRID: return "Hybrid";
        case TrackType::GROUP: return "Group";
        case TrackType::EFFECT: return "Effect";
        case TrackType::MASTER: return "Master";
        default: return "Unknown";
    }
}
