package protocol;

/**
 * TrackType.java - Auto-generated Protocol Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * <p>Type of track in the DAW</p>
 *
 */
public enum TrackType {
    AUDIO(0),
    INSTRUMENT(1),
    HYBRID(2),
    GROUP(3),
    EFFECT(4),
    MASTER(5);

    private final int value;

    TrackType(int value) {
        this.value = value;
    }

    /**
     * Get the integer value for wire encoding.
     * @return The enum value as int
     */
    public int getValue() {
        return value;
    }

    /**
     * Convert from wire value to enum.
     * @param value The integer value from wire
     * @return The corresponding enum value, or AUDIO if unknown
     */
    public static TrackType fromValue(int value) {
        for (TrackType e : values()) {
            if (e.value == value) return e;
        }
        return AUDIO;
    }

    /**
     * Convert from host API string to enum.
     * @param str The string from host API
     * @return The corresponding enum value, or AUDIO if unknown
     */
    public static TrackType fromString(String str) {
        if (str == null) return AUDIO;
        switch (str) {
            case "Audio": return AUDIO;
            case "Instrument": return INSTRUMENT;
            case "Hybrid": return HYBRID;
            case "Group": return GROUP;
            case "Effect": return EFFECT;
            case "Master": return MASTER;
            default: return AUDIO;
        }
    }
}
