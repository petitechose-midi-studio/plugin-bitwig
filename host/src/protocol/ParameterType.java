package protocol;

/**
 * ParameterType.java - Auto-generated Protocol Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * <p>UI hint for parameter rendering</p>
 *
 */
public enum ParameterType {
    KNOB(0),
    BUTTON(1),
    LIST(2);

    private final int value;

    ParameterType(int value) {
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
     * @return The corresponding enum value, or KNOB if unknown
     */
    public static ParameterType fromValue(int value) {
        for (ParameterType e : values()) {
            if (e.value == value) return e;
        }
        return KNOB;
    }
}
