#include "TrackInputHandler.hpp"
#include "InputUtils.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "../../ui/device/DeviceController.hpp"
#include "../../protocol/struct/RequestDeviceListMessage.hpp"
#include "../../protocol/struct/TrackSelectByIndexMessage.hpp"
#include "../../protocol/struct/EnterTrackGroupMessage.hpp"
#include "../../protocol/struct/ExitTrackGroupMessage.hpp"
#include "../../protocol/struct/TrackMuteMessage.hpp"
#include "../../protocol/struct/TrackSoloMessage.hpp"

namespace Bitwig
{

    // =============================================================================
    // Construction / Destruction
    // =============================================================================

    TrackInputHandler::TrackInputHandler(ControllerAPI &api, DeviceView &view,
                                         DeviceController &controller, Protocol::Protocol &protocol)
        : api_(api), view_(view), controller_(controller), protocol_(protocol)
    {
        setupBindings();
    }

    TrackInputHandler::~TrackInputHandler() = default;

    // =============================================================================
    // Public API
    // =============================================================================

    void TrackInputHandler::setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex, bool isNested)
    {
        // Only update cursor if content changed (entered/exited group or first open)
        bool contentChanged = (state_.count != trackCount) || (state_.isNested != isNested);

        state_.count = trackCount;
        state_.currentIndex = currentTrackIndex;
        state_.isNested = isNested;

        if (contentChanged)
        {
            state_.currentSelectorIndex = isNested ? currentTrackIndex + 1 : currentTrackIndex;
            view_.setTrackSelectorIndex(state_.currentSelectorIndex);
        }

        api_.setEncoderMode(EncoderID::NAV, Hardware::EncoderMode::Relative);
    }

    // =============================================================================
    // Bindings Setup
    // =============================================================================

    void TrackInputHandler::setupBindings()
    {
        lv_obj_t *overlay = view_.getTrackSelectorElement();

        // Close track list on press (scoped to overlay)
        api_.onPressed(ButtonID::BOTTOM_LEFT, [this]()
                       { closeAndSelect(); }, overlay);

        // Navigate tracks while holding
        api_.onTurned(EncoderID::NAV, [this](float delta)
                      { navigate(delta); }, overlay);

        // Enter track group
        api_.onPressed(ButtonID::NAV, [this]()
                       { enter(); }, overlay);

        // Mute/Solo
        api_.onPressed(ButtonID::BOTTOM_CENTER, [this]()
                       { toggleMute(); }, overlay);
        api_.onPressed(ButtonID::BOTTOM_RIGHT, [this]()
                       { toggleSolo(); }, overlay);
    }

    // =============================================================================
    // Handlers
    // =============================================================================

    void TrackInputHandler::navigate(float delta)
    {
        int itemCount = view_.getTrackSelectorItemCount();
        if (itemCount == 0)
            return;

        state_.currentSelectorIndex += static_cast<int>(delta);
        state_.currentSelectorIndex = InputUtils::wrapIndex(state_.currentSelectorIndex, itemCount);
        view_.setTrackSelectorIndex(state_.currentSelectorIndex);
    }

    void TrackInputHandler::closeAndSelect()
    {
        if (!view_.isTrackSelectorVisible())
        {
            state_.requested = false;
            return;
        }

        int selectedIndex = state_.currentSelectorIndex;
        view_.hideTrackSelector();
        view_.showDeviceSelector();

        if (state_.isNested && selectedIndex == 0)
        {
            protocol_.send(Protocol::ExitTrackGroupMessage{});
        }
        else
        {
            int trackIndex = getAdjustedTrackIndex(selectedIndex);
            if (trackIndex >= 0 && trackIndex < state_.count)
            {
                protocol_.send(Protocol::TrackSelectByIndexMessage{static_cast<uint8_t>(trackIndex)});
            }
        }

        protocol_.send(Protocol::RequestDeviceListMessage{});
        // Don't reset requested here - DeviceSelectorInputHandler checks it to prevent reopen
    }

    void TrackInputHandler::enter()
    {
        int selectedIndex = state_.currentSelectorIndex;

        if (state_.isNested && selectedIndex == 0)
        {
            // Exit to parent group
            protocol_.send(Protocol::ExitTrackGroupMessage{});
        }
        else
        {
            int trackIndex = getAdjustedTrackIndex(selectedIndex);
            if (trackIndex >= 0 && trackIndex < state_.count)
            {
                // Select the track
                protocol_.send(Protocol::TrackSelectByIndexMessage{static_cast<uint8_t>(trackIndex)});
                // If it's a group, also enter inside
                protocol_.send(Protocol::EnterTrackGroupMessage{static_cast<uint8_t>(trackIndex)});

                // Update current index to match selection
                state_.currentIndex = static_cast<uint8_t>(trackIndex);
            }
        }

        // Refresh track list (will show children if we entered a group)
        protocol_.send(Protocol::RequestTrackListMessage{});
    }

    void TrackInputHandler::toggleMute()
    {
        int trackIndex = getSelectedTrackIndex();
        if (trackIndex >= 0)
        {
            protocol_.send(Protocol::TrackMuteMessage{static_cast<uint8_t>(trackIndex), true});
        }
    }

    void TrackInputHandler::toggleSolo()
    {
        int trackIndex = getSelectedTrackIndex();
        if (trackIndex >= 0)
        {
            protocol_.send(Protocol::TrackSoloMessage{static_cast<uint8_t>(trackIndex), true});
        }
    }

    // =============================================================================
    // Helpers
    // =============================================================================

    int TrackInputHandler::getSelectedTrackIndex() const
    {
        if (state_.isNested && state_.currentSelectorIndex == 0)
            return -1;

        int trackIndex = getAdjustedTrackIndex(state_.currentSelectorIndex);
        return (trackIndex >= 0 && trackIndex < state_.count) ? trackIndex : -1;
    }

    int TrackInputHandler::getAdjustedTrackIndex(int selectorIndex) const
    {
        return state_.isNested ? selectorIndex - 1 : selectorIndex;
    }

} // namespace Bitwig
