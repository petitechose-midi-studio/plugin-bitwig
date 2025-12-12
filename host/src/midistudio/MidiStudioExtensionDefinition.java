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
      // Dev firmware: "MIDI Studio [bitwig:dev]", Release: "MIDI Studio"
      if (platformType == PlatformType.WINDOWS) {
         list.add(new String[] { "MIDI Studio [bitwig:dev]" }, new String[] { "MIDI Studio [bitwig:dev]" });
         list.add(new String[] { "MIDI Studio" }, new String[] { "MIDI Studio" });
      } else if (platformType == PlatformType.MAC) {
         list.add(new String[] { "MIDI Studio [bitwig:dev]" }, new String[] { "MIDI Studio [bitwig:dev]" });
         list.add(new String[] { "MIDI Studio" }, new String[] { "MIDI Studio" });
      } else if (platformType == PlatformType.LINUX) {
         list.add(new String[] { "MIDI Studio [bitwig:dev]" }, new String[] { "MIDI Studio [bitwig:dev]" });
         list.add(new String[] { "MIDI Studio" }, new String[] { "MIDI Studio" });
      }
   }

   @Override
   public MidiStudioExtension createInstance(final ControllerHost host) {
      return new MidiStudioExtension(this, host);
   }
}
