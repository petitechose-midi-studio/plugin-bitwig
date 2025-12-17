package midistudio;

import com.bitwig.extension.controller.api.*;
import com.bitwig.extension.controller.ControllerExtension;
import config.BitwigConfig;
import protocol.Protocol;
import protocol.struct.HostDeactivatedMessage;
import handler.controller.*;
import handler.host.*;

/**
 * MIDI Studio Extension - Entry Point
 *
 * Initializes bidirectional communication with clean separation:
 * - handler/controller: Controller → Bitwig (protocol callbacks)
 * - handler/host: Bitwig → Controller (Bitwig observers)
 *
 * Design: Fire-and-forget pattern
 * - Handlers register callbacks and stay alive via API references
 * - No coupling: Extension doesn't hold references after init
 */
public class MidiStudioExtension extends ControllerExtension {
   private Protocol protocol;

   protected MidiStudioExtension(
         final MidiStudioExtensionDefinition definition,
         final ControllerHost host) {
      super(definition, host);
   }

   @Override
   public void init() {
      final ControllerHost host = getHost();

      Transport transport = host.createTransport();

      CursorTrack cursorTrack = host.createCursorTrack(0, 0);
      CursorDevice cursorDevice = cursorTrack.createCursorDevice();
      CursorRemoteControlsPage remoteControls = cursorDevice.createCursorRemoteControlsPage(8);
      remoteControls.setHardwareLayout(HardwareControlType.ENCODER, 8);

      DeviceBank deviceBank = cursorDevice.deviceChain().createDeviceBank(BitwigConfig.MAX_BANK_SIZE);
      TrackBank trackBank = host.createTrackBank(BitwigConfig.MAX_BANK_SIZE, 8, 0, false);  // 8 sends for MixView
      TrackBank effectTrackBank = host.createEffectTrackBank(8, 0); // For send destination names

      protocol = new Protocol(
            host,
            host.getMidiOutPort(0),
            host.getMidiInPort(0));

      new TransportController(transport, protocol);
      DeviceController deviceController = new DeviceController(host, cursorDevice, remoteControls, protocol, deviceBank,
            transport);

      TransportHost transportHost = new TransportHost(host, protocol, transport);
      transportHost.setupObservers();

      DeviceHost deviceHost = new DeviceHost(host, protocol, cursorTrack, cursorDevice, remoteControls, deviceBank);
      deviceHost.setupObservers();

      deviceController.setDeviceHost(deviceHost);
      deviceHost.setDeviceController(deviceController);

      TrackHost trackHost = new TrackHost(host, protocol, cursorTrack, trackBank, effectTrackBank);
      trackHost.setupObservers();

      TrackController trackController = new TrackController(host, cursorTrack, trackBank, cursorDevice, deviceBank,
            protocol);

      trackController.setTrackHost(trackHost);
      trackController.setDeviceHost(deviceHost);

      // LastClicked: Host (observers) + Controller (protocol callbacks)
      LastClickedHost lastClickedHost = new LastClickedHost(host, protocol);
      LastClickedController lastClickedController = new LastClickedController(host, protocol);
      lastClickedController.setLastClickedHost(lastClickedHost);
      lastClickedHost.setLastClickedController(lastClickedController);
      lastClickedHost.setupObservers();

      // HostStatus: Handles resync requests and initial state
      HostStatusController hostStatusController = new HostStatusController(
            protocol, transportHost, deviceHost, trackHost, lastClickedHost);
      hostStatusController.sendFullState();

      host.showPopupNotification("MIDI Studio : Connected");
   }

   @Override
   public void exit() {
      getHost().showPopupNotification("MIDI Studio : Disconnected");
      // Deactivate first to stop observers, then send final message
      protocol.deactivate();
      protocol.sendFinal(new HostDeactivatedMessage(false));
   }

   @Override
   public void flush() {
   }
}
