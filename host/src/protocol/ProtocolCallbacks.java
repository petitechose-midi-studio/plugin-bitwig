package protocol;

import protocol.struct.*;

/**
 * ProtocolCallbacks - Typed callbacks for all messages
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Base class providing typed callbacks for each message type.
 * Protocol extends this and DecoderRegistry calls these callbacks.
 *
 * Usage:
 *   protocol.onTransportPlay = msg -> {
 *       System.out.println("Playing: " + msg.isPlaying());
 *   };
 */
public class ProtocolCallbacks {

    /**
     * Functional interface for message handlers
     */
    @FunctionalInterface
    public interface MessageHandler<T> {
        void handle(T message);
    }

    // ========================================================================
    // Typed callbacks (one per message)
    // ========================================================================

    public MessageHandler<DeviceChangeMessage> onDeviceChange;
    public MessageHandler<DeviceChangeHeaderMessage> onDeviceChangeHeader;
    public MessageHandler<DeviceChildrenMessage> onDeviceChildren;
    public MessageHandler<DeviceListMessage> onDeviceList;
    public MessageHandler<DevicePageChangeMessage> onDevicePageChange;
    public MessageHandler<DevicePageNamesMessage> onDevicePageNames;
    public MessageHandler<DevicePageSelectByIndexMessage> onDevicePageSelectByIndex;
    public MessageHandler<DeviceRemoteControlDiscreteValuesMessage> onDeviceRemoteControlDiscreteValues;
    public MessageHandler<DeviceRemoteControlNameChangeMessage> onDeviceRemoteControlNameChange;
    public MessageHandler<DeviceRemoteControlTouchMessage> onDeviceRemoteControlTouch;
    public MessageHandler<DeviceRemoteControlUpdateMessage> onDeviceRemoteControlUpdate;
    public MessageHandler<DeviceRemoteControlValueChangeMessage> onDeviceRemoteControlValueChange;
    public MessageHandler<DeviceSelectByIndexMessage> onDeviceSelectByIndex;
    public MessageHandler<DeviceStateChangeMessage> onDeviceStateChange;
    public MessageHandler<EnterDeviceChildMessage> onEnterDeviceChild;
    public MessageHandler<ExitToParentMessage> onExitToParent;
    public MessageHandler<RequestDeviceChildrenMessage> onRequestDeviceChildren;
    public MessageHandler<RequestDeviceListMessage> onRequestDeviceList;
    public MessageHandler<RequestDevicePageNamesMessage> onRequestDevicePageNames;
    public MessageHandler<LastClickedTouchMessage> onLastClickedTouch;
    public MessageHandler<LastClickedUpdateMessage> onLastClickedUpdate;
    public MessageHandler<LastClickedValueChangeMessage> onLastClickedValueChange;
    public MessageHandler<HostDeactivatedMessage> onHostDeactivated;
    public MessageHandler<HostInitializedMessage> onHostInitialized;
    public MessageHandler<RequestHostStatusMessage> onRequestHostStatus;
    public MessageHandler<EnterTrackGroupMessage> onEnterTrackGroup;
    public MessageHandler<ExitTrackGroupMessage> onExitTrackGroup;
    public MessageHandler<RequestTrackListMessage> onRequestTrackList;
    public MessageHandler<TrackActivateMessage> onTrackActivate;
    public MessageHandler<TrackChangeMessage> onTrackChange;
    public MessageHandler<TrackListMessage> onTrackList;
    public MessageHandler<TrackMuteMessage> onTrackMute;
    public MessageHandler<TrackSelectByIndexMessage> onTrackSelectByIndex;
    public MessageHandler<TrackSoloMessage> onTrackSolo;
    public MessageHandler<TransportPlayMessage> onTransportPlay;
    public MessageHandler<TransportRecordMessage> onTransportRecord;
    public MessageHandler<TransportStopMessage> onTransportStop;
    public MessageHandler<TransportTempoMessage> onTransportTempo;

    protected ProtocolCallbacks() {}
}
