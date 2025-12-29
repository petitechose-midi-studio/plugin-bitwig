package protocol;

/**
 * ProtocolConstants - Protocol Configuration Constants (Serial8)
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: Serial8Config
 *
 * This class contains all protocol constants including message structure
 * offsets and encoding limits for 8-bit binary protocol.
 *
 * Note: Framing (COBS) is handled by the bridge layer, not the codec.
 *
 * All constants are public static final (compile-time constants).
 */
public final class ProtocolConstants {

    // Private constructor prevents instantiation (utility class)
    private ProtocolConstants() {
        throw new AssertionError("Utility class cannot be instantiated");
    }

    // ============================================================================
    // MESSAGE STRUCTURE CONSTANTS
    // ============================================================================

    /** Position of MessageID byte in message */
    public static final int MESSAGE_TYPE_OFFSET = 0;

    /** Start of payload data in message */
    public static final int PAYLOAD_OFFSET = 1;

    /** Minimum valid message length */
    public static final int MIN_MESSAGE_LENGTH = 1;

    // ============================================================================
    // ENCODING LIMITS
    // ============================================================================

    /** Maximum characters per string field (8-bit length) */
    public static final int STRING_MAX_LENGTH = 32;

    /** Maximum items per array field (8-bit count) */
    public static final int ARRAY_MAX_ITEMS = 32;

    /** Maximum payload bytes */
    public static final int MAX_PAYLOAD_SIZE = 10000;

    /** Maximum total message bytes */
    public static final int MAX_MESSAGE_SIZE = 16000;


}  // class ProtocolConstants
