package protocol;

/**
 * DeviceType.java - Auto-generated Protocol Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * <p>Type of device in the DAW</p>
 *
 */
public enum DeviceType {
    UNKNOWN(0),
    AUDIO_EFFECT(1),
    INSTRUMENT(2),
    NOTE_EFFECT(3);

    private final int value;

    DeviceType(int value) {
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
     * @return The corresponding enum value, or UNKNOWN if unknown
     */
    public static DeviceType fromValue(int value) {
        for (DeviceType e : values()) {
            if (e.value == value) return e;
        }
        return UNKNOWN;
    }

    /**
     * Convert from host API string to enum.
     * @param str The string from host API
     * @return The corresponding enum value, or UNKNOWN if unknown
     */
    public static DeviceType fromString(String str) {
        if (str == null) return UNKNOWN;
        switch (str) {
            case "audio-effect": return AUDIO_EFFECT;
            case "instrument": return INSTRUMENT;
            case "note-effect": return NOTE_EFFECT;
            default: return UNKNOWN;
        }
    }
}
