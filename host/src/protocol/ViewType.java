package protocol;

/**
 * ViewType.java - Auto-generated Protocol Enum
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * <p>Active view on the controller</p>
 *
 */
public enum ViewType {
    REMOTE_CONTROLS(0),
    MIX(1),
    CLIP(2);

    private final int value;

    ViewType(int value) {
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
     * @return The corresponding enum value, or REMOTE_CONTROLS if unknown
     */
    public static ViewType fromValue(int value) {
        for (ViewType e : values()) {
            if (e.value == value) return e;
        }
        return REMOTE_CONTROLS;
    }
}
