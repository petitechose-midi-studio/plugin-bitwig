package midistudio;

import com.bitwig.extension.controller.api.*;
import com.bitwig.extension.controller.ControllerExtension;
import protocol.Protocol;
import protocol.struct.HostInitializedMessage;
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

      DeviceBank deviceBank = cursorDevice.deviceChain().createDeviceBank(32);
      TrackBank trackBank = host.createTrackBank(32, 0, 0, false);

      protocol = new Protocol(
            host.getMidiOutPort(0),
            host.getMidiInPort(0));

      new TransportController(host, transport, protocol);
      DeviceController deviceController = new DeviceController(host, cursorDevice, remoteControls, protocol, deviceBank,
            transport);

      TransportHost transportHost = new TransportHost(host, protocol, transport);
      transportHost.setup();

      DeviceHost deviceHost = new DeviceHost(host, protocol, cursorTrack, cursorDevice, remoteControls, deviceBank);
      deviceHost.setup();

      deviceController.setDeviceHost(deviceHost);
      deviceHost.setDeviceController(deviceController);

      TrackHost trackHost = new TrackHost(host, protocol, cursorTrack, trackBank);
      trackHost.setup();

      TrackController trackController = new TrackController(host, cursorTrack, trackBank, cursorDevice, deviceBank,
            protocol);

      trackController.setTrackHost(trackHost);
      trackController.setDeviceHost(deviceHost);
      trackController.setDeviceController(deviceController);

      LastClicked lastClickedHost = new LastClicked(host, protocol);
      protocol.send(new HostInitializedMessage(true));
      lastClickedHost.setup();
      transportHost.sendInitialState();
      deviceHost.sendInitialState();
      trackHost.sendInitialState();
      lastClickedHost.sendInitialState();

      protocol.onRequestHostStatus = msg -> {
         protocol.send(new HostInitializedMessage(true));
         transportHost.sendInitialState();
         deviceHost.sendInitialState();
         trackHost.sendInitialState();
         lastClickedHost.sendInitialState();
      };

      host.showPopupNotification("MIDI Studio : Connected");
   }

   @Override
   public void exit() {
      getHost().showPopupNotification("MIDI Studio : Disconnected");
      protocol.send(new HostDeactivatedMessage(false));
      ;
   }

   @Override
   public void flush() {
   }
}
