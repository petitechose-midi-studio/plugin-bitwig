package com.midi_studio.protocol;

import com.midi_studio.protocol.MessageID;
import com.midi_studio.protocol.ProtocolCallbacks;
import com.midi_studio.protocol.struct.*;

/**
 * DecoderRegistry - MessageID to Decoder mapping
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Dispatches incoming messages to typed callbacks.
 * Called by Protocol.dispatch().
 */
public class DecoderRegistry {

    /**
     * Decode message and invoke appropriate callback
     *
     * @param callbacks Object with typed callbacks (ProtocolCallbacks)
     * @param messageId MessageID to decode
     * @param payload Raw payload bytes
     * @param fromHost Origin flag (true if message from host, false if from controller)
     */
    public static void dispatch(
        ProtocolCallbacks callbacks,
        MessageID messageId,
        byte[] payload,
        boolean fromHost
    ) {
        switch (messageId) {
            case DEVICE_CHANGE:
                if (callbacks.onDeviceChange != null) {
                    DeviceChangeMessage msg = DeviceChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChange.handle(msg);
                }
                break;
            case DEVICE_CHANGE_HEADER:
                if (callbacks.onDeviceChangeHeader != null) {
                    DeviceChangeHeaderMessage msg = DeviceChangeHeaderMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChangeHeader.handle(msg);
                }
                break;
            case DEVICE_CHILDREN:
                if (callbacks.onDeviceChildren != null) {
                    DeviceChildrenMessage msg = DeviceChildrenMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceChildren.handle(msg);
                }
                break;
            case DEVICE_LIST:
                if (callbacks.onDeviceList != null) {
                    DeviceListMessage msg = DeviceListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceList.handle(msg);
                }
                break;
            case DEVICE_MACRO_DISCRETE_VALUES:
                if (callbacks.onDeviceMacroDiscreteValues != null) {
                    DeviceMacroDiscreteValuesMessage msg = DeviceMacroDiscreteValuesMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroDiscreteValues.handle(msg);
                }
                break;
            case DEVICE_MACRO_NAME_CHANGE:
                if (callbacks.onDeviceMacroNameChange != null) {
                    DeviceMacroNameChangeMessage msg = DeviceMacroNameChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroNameChange.handle(msg);
                }
                break;
            case DEVICE_MACRO_TOUCH:
                if (callbacks.onDeviceMacroTouch != null) {
                    DeviceMacroTouchMessage msg = DeviceMacroTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroTouch.handle(msg);
                }
                break;
            case DEVICE_MACRO_UPDATE:
                if (callbacks.onDeviceMacroUpdate != null) {
                    DeviceMacroUpdateMessage msg = DeviceMacroUpdateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroUpdate.handle(msg);
                }
                break;
            case DEVICE_MACRO_VALUE_CHANGE:
                if (callbacks.onDeviceMacroValueChange != null) {
                    DeviceMacroValueChangeMessage msg = DeviceMacroValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceMacroValueChange.handle(msg);
                }
                break;
            case DEVICE_PAGE_CHANGE:
                if (callbacks.onDevicePageChange != null) {
                    DevicePageChangeMessage msg = DevicePageChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageChange.handle(msg);
                }
                break;
            case DEVICE_PAGE_NAMES:
                if (callbacks.onDevicePageNames != null) {
                    DevicePageNamesMessage msg = DevicePageNamesMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageNames.handle(msg);
                }
                break;
            case DEVICE_PAGE_SELECT_BY_INDEX:
                if (callbacks.onDevicePageSelectByIndex != null) {
                    DevicePageSelectByIndexMessage msg = DevicePageSelectByIndexMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDevicePageSelectByIndex.handle(msg);
                }
                break;
            case DEVICE_SELECT_BY_INDEX:
                if (callbacks.onDeviceSelectByIndex != null) {
                    DeviceSelectByIndexMessage msg = DeviceSelectByIndexMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceSelectByIndex.handle(msg);
                }
                break;
            case DEVICE_STATE_CHANGE:
                if (callbacks.onDeviceStateChange != null) {
                    DeviceStateChangeMessage msg = DeviceStateChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onDeviceStateChange.handle(msg);
                }
                break;
            case ENTER_DEVICE_CHILD:
                if (callbacks.onEnterDeviceChild != null) {
                    EnterDeviceChildMessage msg = EnterDeviceChildMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onEnterDeviceChild.handle(msg);
                }
                break;
            case EXIT_TO_PARENT:
                if (callbacks.onExitToParent != null) {
                    ExitToParentMessage msg = ExitToParentMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onExitToParent.handle(msg);
                }
                break;
            case REQUEST_DEVICE_CHILDREN:
                if (callbacks.onRequestDeviceChildren != null) {
                    RequestDeviceChildrenMessage msg = RequestDeviceChildrenMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceChildren.handle(msg);
                }
                break;
            case REQUEST_DEVICE_LIST:
                if (callbacks.onRequestDeviceList != null) {
                    RequestDeviceListMessage msg = RequestDeviceListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDeviceList.handle(msg);
                }
                break;
            case REQUEST_DEVICE_PAGE_NAMES:
                if (callbacks.onRequestDevicePageNames != null) {
                    RequestDevicePageNamesMessage msg = RequestDevicePageNamesMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestDevicePageNames.handle(msg);
                }
                break;
            case LAST_CLICKED_TOUCH:
                if (callbacks.onLastClickedTouch != null) {
                    LastClickedTouchMessage msg = LastClickedTouchMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedTouch.handle(msg);
                }
                break;
            case LAST_CLICKED_UPDATE:
                if (callbacks.onLastClickedUpdate != null) {
                    LastClickedUpdateMessage msg = LastClickedUpdateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedUpdate.handle(msg);
                }
                break;
            case LAST_CLICKED_VALUE_CHANGE:
                if (callbacks.onLastClickedValueChange != null) {
                    LastClickedValueChangeMessage msg = LastClickedValueChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onLastClickedValueChange.handle(msg);
                }
                break;
            case HOST_DEACTIVATED:
                if (callbacks.onHostDeactivated != null) {
                    HostDeactivatedMessage msg = HostDeactivatedMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onHostDeactivated.handle(msg);
                }
                break;
            case HOST_INITIALIZED:
                if (callbacks.onHostInitialized != null) {
                    HostInitializedMessage msg = HostInitializedMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onHostInitialized.handle(msg);
                }
                break;
            case REQUEST_HOST_STATUS:
                if (callbacks.onRequestHostStatus != null) {
                    RequestHostStatusMessage msg = RequestHostStatusMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestHostStatus.handle(msg);
                }
                break;
            case ENTER_TRACK_GROUP:
                if (callbacks.onEnterTrackGroup != null) {
                    EnterTrackGroupMessage msg = EnterTrackGroupMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onEnterTrackGroup.handle(msg);
                }
                break;
            case EXIT_TRACK_GROUP:
                if (callbacks.onExitTrackGroup != null) {
                    ExitTrackGroupMessage msg = ExitTrackGroupMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onExitTrackGroup.handle(msg);
                }
                break;
            case REQUEST_TRACK_LIST:
                if (callbacks.onRequestTrackList != null) {
                    RequestTrackListMessage msg = RequestTrackListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onRequestTrackList.handle(msg);
                }
                break;
            case TRACK_ACTIVATE:
                if (callbacks.onTrackActivate != null) {
                    TrackActivateMessage msg = TrackActivateMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackActivate.handle(msg);
                }
                break;
            case TRACK_CHANGE:
                if (callbacks.onTrackChange != null) {
                    TrackChangeMessage msg = TrackChangeMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackChange.handle(msg);
                }
                break;
            case TRACK_LIST:
                if (callbacks.onTrackList != null) {
                    TrackListMessage msg = TrackListMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackList.handle(msg);
                }
                break;
            case TRACK_MUTE:
                if (callbacks.onTrackMute != null) {
                    TrackMuteMessage msg = TrackMuteMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackMute.handle(msg);
                }
                break;
            case TRACK_SELECT_BY_INDEX:
                if (callbacks.onTrackSelectByIndex != null) {
                    TrackSelectByIndexMessage msg = TrackSelectByIndexMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSelectByIndex.handle(msg);
                }
                break;
            case TRACK_SOLO:
                if (callbacks.onTrackSolo != null) {
                    TrackSoloMessage msg = TrackSoloMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTrackSolo.handle(msg);
                }
                break;
            case TRANSPORT_PLAY:
                if (callbacks.onTransportPlay != null) {
                    TransportPlayMessage msg = TransportPlayMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportPlay.handle(msg);
                }
                break;
            case TRANSPORT_RECORD:
                if (callbacks.onTransportRecord != null) {
                    TransportRecordMessage msg = TransportRecordMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportRecord.handle(msg);
                }
                break;
            case TRANSPORT_STOP:
                if (callbacks.onTransportStop != null) {
                    TransportStopMessage msg = TransportStopMessage.decode(payload);
                    msg.fromHost = fromHost;  // Inject origin flag
                    callbacks.onTransportStop.handle(msg);
                }
                break;
            default:
                // Unknown message type - silently ignore
                break;
        }
    }

    // Utility class - prevent instantiation
    private DecoderRegistry() {}
}
