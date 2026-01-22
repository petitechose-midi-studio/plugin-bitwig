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
 * - handler/controller: Controller -> Bitwig (protocol callbacks)
 * - handler/host: Bitwig -> Controller (Bitwig observers)
 *
 * Design: Fire-and-forget pattern
 * - Handlers register callbacks and stay alive via API references
 * - No coupling: Extension doesn't hold references after init
 */
public class MidiStudioExtension extends ControllerExtension {
   private Protocol protocol;

   // Bridge mode setting (enum dropdown)
   private static final String[] BRIDGE_MODES = {
       "Hardware (9000)",
       "Native Sim (9001)",
       "WASM Sim (9002)"
   };

   protected MidiStudioExtension(
         final MidiStudioExtensionDefinition definition,
         final ControllerHost host) {
      super(definition, host);
   }

   @Override
   public void init() {
      final ControllerHost host = getHost();

      // Bridge mode setting (dropdown: Hardware/Native/WASM)
      // Enum settings work correctly with .get() during init (unlike number settings with .getRaw())
      final SettableEnumValue bridgeModeSetting = host.getPreferences()
         .getEnumSetting("Bridge Mode", "Connection", BRIDGE_MODES, "Hardware (9000)");
      bridgeModeSetting.markInterested();

      // Parse port from enum setting
      final int bridgePort = switch(bridgeModeSetting.get()) {
          case "Hardware (9000)" -> 9000;
          case "Native Sim (9001)" -> 9001;
          case "WASM Sim (9002)" -> 9002;
          default -> 9000;
      };

      Transport transport = host.createTransport();

      CursorTrack cursorTrack = host.createCursorTrack(0, 0);
      CursorDevice cursorDevice = cursorTrack.createCursorDevice();
      CursorRemoteControlsPage remoteControls = cursorDevice.createCursorRemoteControlsPage(8);
      remoteControls.setHardwareLayout(HardwareControlType.ENCODER, 8);

      DeviceBank deviceBank = cursorDevice.deviceChain().createDeviceBank(BitwigConfig.MAX_BANK_SIZE);
      TrackBank trackBank = host.createTrackBank(BitwigConfig.MAX_BANK_SIZE, 8, 0, false);  // 8 sends for MixView
      TrackBank effectTrackBank = host.createEffectTrackBank(8, 0); // For send destination names

      protocol = new Protocol(host, "127.0.0.1", bridgePort);

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

      host.showPopupNotification("MIDI Studio : Connected (port " + bridgePort + ")");
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
