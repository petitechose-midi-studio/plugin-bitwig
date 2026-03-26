package midistudio;

import java.util.UUID;

import com.bitwig.extension.api.PlatformType;
import com.bitwig.extension.controller.AutoDetectionMidiPortNamesList;
import com.bitwig.extension.controller.ControllerExtensionDefinition;
import com.bitwig.extension.controller.api.ControllerHost;

public class MidiStudioExtensionDefinition extends ControllerExtensionDefinition {
   private static final UUID DRIVER_ID = UUID.fromString("c7a088c8-792c-40e5-8997-61bd0e332146");

   public MidiStudioExtensionDefinition() {
   }

   @Override
   public String getName() {
      return "MIDI Studio";
   }

   @Override
   public String getAuthor() {
      return "petitechose.audio";
   }

   @Override
   public String getVersion() {
      return "0.1";
   }

   @Override
   public UUID getId() {
      return DRIVER_ID;
   }

   @Override
   public String getHardwareVendor() {
      return "petitechose.audio";
   }

   @Override
   public String getHardwareModel() {
      return "midi_studio";
   }

   @Override
   public int getRequiredAPIVersion() {
      return 25;
   }

   @Override
   public int getNumMidiInPorts() {
      return 1;
   }

   @Override
   public int getNumMidiOutPorts() {
      return 1;
   }

   @Override
   public void listAutoDetectionMidiPortNames(final AutoDetectionMidiPortNamesList list,
         final PlatformType platformType) {
      // Keep hardware MIDI ports manual.
      //
      // Multiple hardware controllers expose the same USB MIDI display name
      // ("MIDI Studio [hw]"), so name-based auto-detection is ambiguous.
      // The extension still requests one MIDI input and one MIDI output so
      // future note/CC features remain available, but the user must assign the
      // hardware ports explicitly for multi-controller setups.

      // Native desktop (virtual ports via libremidi)
      list.add(new String[] { "MIDI Studio [native] IN" }, new String[] { "MIDI Studio [native] OUT" });

      // WASM browser (virtual ports, must be created externally)
      list.add(new String[] { "MIDI Studio [wasm] IN" }, new String[] { "MIDI Studio [wasm] OUT" });
   }

   @Override
   public MidiStudioExtension createInstance(final ControllerHost host) {
      return new MidiStudioExtension(this, host);
   }
}
