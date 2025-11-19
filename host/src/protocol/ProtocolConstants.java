package protocol;

/**
 * ProtocolConstants - Protocol Configuration Constants
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: protocol_config.yaml
 *
 * This class contains all protocol constants including SysEx framing,
 * message structure offsets, and encoding limits.
 *
 * All constants are public static final (compile-time constants).
 */
public final class ProtocolConstants {

    // Private constructor prevents instantiation (utility class)
    private ProtocolConstants() {
        throw new AssertionError("Utility class cannot be instantiated");
    }

    // ============================================================================
    // SYSEX FRAMING CONSTANTS
    // ============================================================================

    /** SysEx start byte */
    public static final byte SYSEX_START = (byte) 0xf0;

    /** SysEx end byte */
    public static final byte SYSEX_END = (byte) 0xf7;

    /** MIDI manufacturer ID */
    public static final byte MANUFACTURER_ID = 0x7f;

    /** Device identifier */
    public static final byte DEVICE_ID = 0x01;

    /** Minimum valid SysEx message length */
    public static final int MIN_MESSAGE_LENGTH = 6;

    /** Position of MessageID byte in SysEx message */
    public static final int MESSAGE_TYPE_OFFSET = 3;

    /** Position of fromHost flag in SysEx message */
    public static final int FROM_HOST_OFFSET = 4;

    /** Start of payload data in SysEx message */
    public static final int PAYLOAD_OFFSET = 5;

    // ============================================================================
    // ENCODING LIMITS
    // ============================================================================

    /** Maximum characters per string field */
    public static final int STRING_MAX_LENGTH = 16;

    /** Maximum items per array field */
    public static final int ARRAY_MAX_ITEMS = 32;

    /** Maximum payload bytes */
    public static final int MAX_PAYLOAD_SIZE = 256;

    /** Maximum total message bytes */
    public static final int MAX_MESSAGE_SIZE = 512;

    // ============================================================================
    // MESSAGE ID RANGES
    // ============================================================================

    /** Controller → Host range start */
    public static final byte ID_RANGE_CONTROLLER_TO_HOST_START = 0x00;

    /** Controller → Host range end */
    public static final byte ID_RANGE_CONTROLLER_TO_HOST_END = 0x3f;

    /** Host → Controller range start */
    public static final byte ID_RANGE_HOST_TO_CONTROLLER_START = 0x40;

    /** Host → Controller range end */
    public static final byte ID_RANGE_HOST_TO_CONTROLLER_END = (byte) 0xbf;

    /** Bidirectional range start */
    public static final byte ID_RANGE_BIDIRECTIONAL_START = (byte) 0xc0;

    /** Bidirectional range end */
    public static final byte ID_RANGE_BIDIRECTIONAL_END = (byte) 0xff;

    // ============================================================================
    // ROLE CONFIGURATION
    // ============================================================================

    /** This code's role in the protocol */
    public static final boolean IS_HOST = true;


}  // class ProtocolConstants
