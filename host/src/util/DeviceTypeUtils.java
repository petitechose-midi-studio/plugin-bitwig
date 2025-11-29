package util;

/**
 * DeviceTypeUtils - Convert Bitwig device type strings to protocol integers
 *
 * Used by DeviceHost to encode device types for the protocol.
 */
public final class DeviceTypeUtils {

    private DeviceTypeUtils() {} // Prevent instantiation

    // Device type constants (matching C++ enum)
    public static final int UNKNOWN = 0;
    public static final int AUDIO_EFFECT = 1;
    public static final int INSTRUMENT = 2;
    public static final int NOTE_EFFECT = 3;

    /**
     * Convert Bitwig device type string to uint8
     * @param deviceType Device type string from Bitwig API (device.deviceType().get())
     * @return 0=Unknown, 1=AudioEffect, 2=Instrument, 3=NoteEffect
     */
    public static int toInt(String deviceType) {
        if (deviceType == null) return UNKNOWN;

        switch (deviceType) {
            case "audio-effect":
                return AUDIO_EFFECT;
            case "instrument":
                return INSTRUMENT;
            case "note-effect":
                return NOTE_EFFECT;
            default:
                return UNKNOWN;
        }
    }
}
