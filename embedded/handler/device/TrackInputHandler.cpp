#include "TrackInputHandler.hpp"
#include "../../ui/device/DeviceView.hpp"
#include "../../ui/device/DeviceController.hpp"
#include "../../ui/device/component/TrackListSelector.hpp"
#include "../../protocol/struct/RequestTrackListMessage.hpp"
#include "../../protocol/struct/TrackSelectByIndexMessage.hpp"
#include "../../protocol/struct/EnterTrackGroupMessage.hpp"
#include "../../protocol/struct/ExitTrackGroupMessage.hpp"
#include "../../protocol/struct/TrackMuteMessage.hpp"
#include "../../protocol/struct/TrackSoloMessage.hpp"
#include "log/Macros.hpp"

namespace Bitwig
{

    TrackInputHandler::TrackInputHandler(ControllerAPI &api, DeviceView &view,
                                         DeviceController &controller, Protocol::Protocol &protocol,
                                         lv_obj_t *scope)
        : api_(api), view_(view), scope_(scope), view_controller_(controller), protocol_(protocol)
    {
        setupBindings();
    }

    TrackInputHandler::~TrackInputHandler()
    {
        // Bindings are automatically cleaned up by ControllerAPI when scope is destroyed
    }

    int TrackInputHandler::wrapIndex(int value, int modulo)
    {
        return ((value % modulo) + modulo) % modulo;
    }

    void TrackInputHandler::setTrackListState(uint8_t trackCount, uint8_t currentTrackIndex, bool isNested)
    {
        trackList_.count = trackCount;
        trackList_.currentIndex = currentTrackIndex;
        trackList_.isNested = isNested;

        // Adjust initial encoder position for nested mode (when "Back to parent" is at index 0)
        int initialPosition = trackList_.isNested ? currentTrackIndex + 1 : currentTrackIndex;
        api_.setEncoderPosition(EncoderID::NAV, initialPosition);
    }

    void TrackInputHandler::setupBindings()
    {
        // All bindings scoped to TrackListSelector overlay
        lv_obj_t *trackSelectorScope = view_.getTrackListSelector().getElement();

        // Navigation with encoder while BOTTOM_LEFT is pressed
        api_.onTurnedWhilePressed(
            EncoderID::NAV,
            ButtonID::BOTTOM_LEFT,
            [this](float position)
            {
                handleTrackSelectorNavigation(position);
            },
            trackSelectorScope);

        // Release BOTTOM_LEFT: select track and close overlay
        api_.onReleased(
            ButtonID::BOTTOM_LEFT,
            [this]()
            {
                handleTrackSelectorRelease();
            },
            trackSelectorScope);

        // Encoder NAV press: Enter track group (if it's a group)
        api_.onCombo(
            ButtonID::NAV,
            ButtonID::BOTTOM_LEFT,
            [this]()
            {
                handleTrackSelectorEnter();
            },
            trackSelectorScope);

        // BOTTOM_CENTER: Mute selected track
        api_.onPressed(
            ButtonID::BOTTOM_CENTER,
            [this]()
            {
                handleTrackMute();
            },
            trackSelectorScope);

        // BOTTOM_RIGHT: Solo selected track
        api_.onPressed(
            ButtonID::BOTTOM_RIGHT,
            [this]()
            {
                handleTrackSolo();
            },
            trackSelectorScope);
    }

    void TrackInputHandler::handleTrackSelectorNavigation(float position)
    {
        int itemCount = view_.getTrackListSelector().getItemCount();
        if (itemCount == 0)
            return;

        int wrappedIndex = wrapIndex(position, itemCount);
        view_.getTrackListSelector().setSelectedIndex(wrappedIndex);
    }

    void TrackInputHandler::handleTrackSelectorRelease()
    {
        // If track selector was already closed (device selector button was released first), do nothing
        if (!view_.getTrackListSelector().isVisible())
        {
            return;
        }

        const int selectedIndex = view_.getTrackListSelector().getSelectedIndex();

        view_.getTrackListSelector().hide();
        view_.getDeviceSelector().show();

        // Check if "Back to parent" was selected (index 0 when nested)
        if (trackList_.isNested && selectedIndex == 0)
        {
            protocol_.send(Protocol::ExitTrackGroupMessage{});
        }
        else
        {
            // Adjust index if nested (skip "Back to parent")
            int trackIndex = trackList_.isNested ? (selectedIndex - 1) : selectedIndex;

            if (trackIndex >= 0 && trackIndex < trackList_.count)
            {
                // Select track - Java will automatically send updated device list
                protocol_.send(Protocol::TrackSelectByIndexMessage{static_cast<uint8_t>(trackIndex)});
            }
        }

        // Reset encoder position
        api_.setEncoderPosition(EncoderID::NAV, 0.0f);
    }

    void TrackInputHandler::handleTrackSelectorEnter()
    {
        const int selectedIndex = view_.getTrackListSelector().getSelectedIndex();

        if (trackList_.isNested && selectedIndex == 0)
        {
            // "Back to parent" - exit group
            protocol_.send(Protocol::ExitTrackGroupMessage{});
        }
        else
        {
            // Adjust index if nested (skip "Back to parent")
            int trackIndex = trackList_.isNested ? (selectedIndex - 1) : selectedIndex;

            if (trackIndex >= 0 && trackIndex < trackList_.count)
            {
                // TODO: Check if track is a group before entering
                // For now, always send enter request (host will handle if not a group)
                protocol_.send(Protocol::EnterTrackGroupMessage{static_cast<uint8_t>(trackIndex)});
            }
        }
    }

    void TrackInputHandler::handleTrackMute()
    {
        const int selectedIndex = view_.getTrackListSelector().getSelectedIndex();

        // Don't mute "Back to parent"
        if (trackList_.isNested && selectedIndex == 0)
            return;

        int trackIndex = trackList_.isNested ? (selectedIndex - 1) : selectedIndex;

        if (trackIndex >= 0 && trackIndex < trackList_.count)
        {
            // Send toggle request (isMute=true means toggle, Bitwig will respond with actual state)
            protocol_.send(Protocol::TrackMuteMessage{static_cast<uint8_t>(trackIndex), true});
        }
    }

    void TrackInputHandler::handleTrackSolo()
    {
        const int selectedIndex = view_.getTrackListSelector().getSelectedIndex();

        // Don't solo "Back to parent"
        if (trackList_.isNested && selectedIndex == 0)
            return;

        int trackIndex = trackList_.isNested ? (selectedIndex - 1) : selectedIndex;

        if (trackIndex >= 0 && trackIndex < trackList_.count)
        {
            // Send toggle request (isSolo=true means toggle, Bitwig will respond with actual state)
            protocol_.send(Protocol::TrackSoloMessage{static_cast<uint8_t>(trackIndex), true});
        }
    }

} // namespace Bitwig
