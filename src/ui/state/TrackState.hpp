#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Bitwig {

/**
 * @brief State for track list and selection
 */
struct TrackState {
    struct TrackInfo {
        std::string name;
        uint32_t color = 0xFFFFFF;
        uint8_t type = 0;  // 0=audio, 1=instrument, 2=hybrid, 3=group, 4=return, 5=master
        bool muted = false;
        bool soloed = false;
        float level = 0.0f;
    };

    std::vector<TrackInfo> tracks;
    int currentIndex = 0;
    bool selectorVisible = false;

    // Dirty tracking
    bool dirty = false;

    void markDirty() { dirty = true; }
    void clearDirty() { dirty = false; }

    // Helper to update from separate vectors (for compatibility with existing API)
    void updateFromVectors(const std::vector<std::string>& names,
                           const std::vector<uint32_t>& colors,
                           const std::vector<uint8_t>& types,
                           const std::vector<bool>& muteStates,
                           const std::vector<bool>& soloStates) {
        tracks.clear();
        tracks.reserve(names.size());
        for (size_t i = 0; i < names.size(); ++i) {
            TrackInfo info;
            info.name = names[i];
            info.color = (i < colors.size()) ? colors[i] : 0xFFFFFF;
            info.type = (i < types.size()) ? types[i] : 0;
            info.muted = (i < muteStates.size()) ? muteStates[i] : false;
            info.soloed = (i < soloStates.size()) ? soloStates[i] : false;
            tracks.push_back(std::move(info));
        }
        markDirty();
    }
};

}  // namespace Bitwig
