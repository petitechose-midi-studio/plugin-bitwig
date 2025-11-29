package util;

/**
 * TrackTypeUtils - Convert Bitwig track type strings to protocol integers
 *
 * Shared by DeviceHost and TrackHost to avoid duplication.
 */
public final class TrackTypeUtils {

    private TrackTypeUtils() {} // Prevent instantiation

    /**
     * Convert Bitwig track type string to uint8
     * @param trackType Track type string from Bitwig API
     * @return 0=Audio, 1=Instrument, 2=Hybrid, 3=Group, 4=Effect, 5=Master
     */
    public static int toInt(String trackType) {
        switch (trackType) {
            case "Audio": return 0;
            case "Instrument": return 1;
            case "Hybrid": return 2;
            case "Group": return 3;
            case "Effect": return 4;
            case "Master": return 5;
            default: return 0;
        }
    }
}
