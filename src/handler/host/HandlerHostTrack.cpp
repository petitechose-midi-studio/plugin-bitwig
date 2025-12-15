#include "HandlerHostTrack.hpp"

#include <vector>

#include "handler/NestedIndexUtils.hpp"
#include "protocol/struct/TrackChangeMessage.hpp"
#include "protocol/struct/TrackListMessage.hpp"
#include "protocol/struct/TrackMuteMessage.hpp"
#include "protocol/struct/TrackSoloMessage.hpp"
#include "state/Constants.hpp"

namespace bitwig::handler {

using namespace Protocol;
using namespace bitwig::state;
constexpr auto BACK_TO_PARENT = BACK_TO_PARENT_TEXT;

HandlerHostTrack::HandlerHostTrack(state::BitwigState& state, BitwigProtocol& protocol)
    : state_(state), protocol_(protocol) {
    setupProtocolCallbacks();
}

void HandlerHostTrack::setupProtocolCallbacks() {
    protocol_.onTrackChange = [this](const TrackChangeMessage& msg) {
        state_.currentTrack.name.set(msg.trackName.c_str());
        state_.currentTrack.color.set(msg.color);
        state_.currentTrack.trackType.set(static_cast<state::TrackType>(msg.trackType));
    };

    protocol_.onTrackList = [this](const TrackListMessage& msg) {
        if (!msg.fromHost) return;

        // Update navigation state
        state_.trackSelector.isNested.set(msg.isNested);
        state_.trackSelector.activeTrackIndex.set(msg.trackIndex);

        // Build temporary lists for bulk data
        std::vector<std::string> names;
        std::vector<uint8_t> trackTypes;
        std::vector<uint32_t> trackColors;

        uint8_t displayIndex = 0;
        if (msg.isNested) {
            names.push_back(BACK_TO_PARENT);
            trackTypes.push_back(0);
            trackColors.push_back(0xFFFFFF);
            state_.trackSelector.muteStates[displayIndex].set(false);
            state_.trackSelector.soloStates[displayIndex].set(false);
            displayIndex++;
        }

        for (uint8_t i = 0; i < msg.trackCount && displayIndex < MAX_TRACKS; i++) {
            names.push_back(std::string(msg.tracks[i].trackName.data()));
            trackTypes.push_back(msg.tracks[i].trackType);
            trackColors.push_back(msg.tracks[i].color);
            state_.trackSelector.muteStates[displayIndex].set(msg.tracks[i].isMute);
            state_.trackSelector.soloStates[displayIndex].set(msg.tracks[i].isSolo);
            displayIndex++;
        }

        // Update SignalVectors (bulk data)
        state_.trackSelector.names.set(names.data(), names.size());
        state_.trackSelector.trackTypes.set(trackTypes.data(), trackTypes.size());
        state_.trackSelector.trackColors.set(trackColors.data(), trackColors.size());

        state_.trackSelector.currentIndex.set(msg.isNested ? msg.trackIndex + 1 : msg.trackIndex);
        // NOTE: visibility is controlled by input handlers, not host handlers
    };

    protocol_.onTrackMute = [this](const TrackMuteMessage& msg) {
        if (!msg.fromHost) return;

        int displayIndex = utils::toDisplayIndex(msg.trackIndex, state_.trackSelector.isNested.get());
        if (displayIndex >= 0 && displayIndex < MAX_TRACKS) {
            state_.trackSelector.muteStates[displayIndex].set(msg.isMute);
        }
    };

    protocol_.onTrackSolo = [this](const TrackSoloMessage& msg) {
        if (!msg.fromHost) return;

        int displayIndex = utils::toDisplayIndex(msg.trackIndex, state_.trackSelector.isNested.get());
        if (displayIndex >= 0 && displayIndex < MAX_TRACKS) {
            state_.trackSelector.soloStates[displayIndex].set(msg.isSolo);
        }
    };
}

}  // namespace bitwig::handler
