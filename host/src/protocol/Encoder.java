package protocol;

/**
 * Encoder - 8-bit Binary Encoder (Serial8 Protocol)
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This class provides static encode methods for all builtin primitive
 * types. Uses full 8-bit byte range for efficient encoding.
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, norm8, norm16, string
 *
 * Encoding Strategy:
 * - Direct byte writes (no 7-bit constraint)
 * - Little-endian for multi-byte integers
 * - IEEE 754 for floats
 * - 8-bit length prefix for strings/arrays
 *
 * Performance:
 * - Static methods for zero overhead
 * - Direct byte array manipulation
 * - More efficient than 7-bit encoding (no expansion)
 */
public final class Encoder {

    // Private constructor prevents instantiation (utility class)
    private Encoder() {
        throw new AssertionError("Utility class cannot be instantiated");
    }

    // ============================================================================
    // ENCODE METHODS (Type -> 8-bit binary bytes)
    // ============================================================================


    /**
     * Encode bool (1 byte: 0x00 or 0x01)
     * Boolean value (true/false)
     *
     * @param value Value to encode
     * @return Encoded byte array (1 byte)
     */
    public static byte[] encodeBool(boolean value) {
        return new byte[]{ (byte) (value ? 0x01 : 0x00) };
    }


    /**
     * Encode float32 (4 bytes, IEEE 754 little-endian)
     * 32-bit IEEE 754 floating point
     *
     * @param value Value to encode
     * @return Encoded byte array (4 bytes)
     */
    public static byte[] encodeFloat32(float value) {
        int bits = Float.floatToRawIntBits(value);
        return new byte[]{
            (byte) (bits & 0xFF),
            (byte) ((bits >> 8) & 0xFF),
            (byte) ((bits >> 16) & 0xFF),
            (byte) ((bits >> 24) & 0xFF)
        };
    }


    /**
     * Encode int16 (2 bytes, little-endian)
     * 16-bit signed integer (-32768 to 32767)
     *
     * @param value Value to encode
     * @return Encoded byte array (2 bytes)
     */
    public static byte[] encodeInt16(short value) {
        int bits = value & 0xFFFF;
        return new byte[]{
            (byte) (bits & 0xFF),
            (byte) ((bits >> 8) & 0xFF)
        };
    }


    /**
     * Encode int32 (4 bytes, little-endian)
     * 32-bit signed integer (-2147483648 to 2147483647)
     *
     * @param value Value to encode
     * @return Encoded byte array (4 bytes)
     */
    public static byte[] encodeInt32(int value) {
        return new byte[]{
            (byte) (value & 0xFF),
            (byte) ((value >> 8) & 0xFF),
            (byte) ((value >> 16) & 0xFF),
            (byte) ((value >> 24) & 0xFF)
        };
    }


    /**
     * Encode int8 (1 byte, direct)
     * 8-bit signed integer (-128 to 127)
     *
     * @param value Value to encode
     * @return Encoded byte array (1 byte)
     */
    public static byte[] encodeInt8(byte value) {
        return new byte[]{ (byte) value };
    }


    /**
     * Encode norm16 (2 bytes, little-endian)
     * Normalized float (0.0-1.0) stored as uint16 for efficiency
     *
     * Converts float 0.0-1.0 to uint16 0-65535 for high precision.
     * Precision: ~0.0015% (1/65535)
     *
     * @param value Float value to encode (clamped to 0.0-1.0)
     * @return Encoded byte array (2 bytes)
     */
    public static byte[] encodeNorm16(float value) {
        float clamped = Math.max(0.0f, Math.min(1.0f, value));
        int val = Math.round(clamped * 65535.0f) & 0xFFFF;
        return new byte[]{
            (byte) (val & 0xFF),
            (byte) ((val >> 8) & 0xFF)
        };
    }


    /**
     * Encode norm8 (1 byte, full 8-bit range)
     * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth
     *
     * Converts float 0.0-1.0 to 8-bit value 0-255 for optimal precision.
     * Precision: ~0.4% (1/255), better than 7-bit norm8.
     *
     * @param value Float value to encode (clamped to 0.0-1.0)
     * @return Encoded byte array (1 byte)
     */
    public static byte[] encodeNorm8(float value) {
        float clamped = Math.max(0.0f, Math.min(1.0f, value));
        int val = Math.round(clamped * 255.0f) & 0xFF;
        return new byte[]{ (byte) val };
    }


    /**
     * Encode string (variable length: 1 byte length + data)
     * Variable-length UTF-8 string (prefixed with uint8 length)
     *
     * Format: [length (8-bit)] [char0] [char1] ... [charN-1]
     * Max length: 255 chars (8-bit length encoding)
     *
     * @param value String to encode
     * @param maxLength Maximum allowed string length
     * @return Encoded byte array (1 + string.length bytes)
     */
    public static byte[] encodeString(String value, int maxLength) {
        if (value == null) {
            value = "";
        }

        int len = Math.min(value.length(), Math.min(maxLength, 255));
        byte[] result = new byte[1 + len];

        result[0] = (byte) len;

        for (int i = 0; i < len; i++) {
            result[1 + i] = (byte) value.charAt(i);
        }

        return result;
    }


    /**
     * Encode uint16 (2 bytes, little-endian)
     * 16-bit unsigned integer (0-65535)
     *
     * @param value Value to encode (treated as unsigned)
     * @return Encoded byte array (2 bytes)
     */
    public static byte[] encodeUint16(int value) {
        int val = value & 0xFFFF;
        return new byte[]{
            (byte) (val & 0xFF),         // low byte
            (byte) ((val >> 8) & 0xFF)   // high byte
        };
    }


    /**
     * Encode uint32 (4 bytes, little-endian)
     * 32-bit unsigned integer (0-4294967295)
     *
     * @param value Value to encode (treated as unsigned)
     * @return Encoded byte array (4 bytes)
     */
    public static byte[] encodeUint32(long value) {
        long val = value & 0xFFFFFFFFL;
        return new byte[]{
            (byte) (val & 0xFF),
            (byte) ((val >> 8) & 0xFF),
            (byte) ((val >> 16) & 0xFF),
            (byte) ((val >> 24) & 0xFF)
        };
    }


    /**
     * Encode uint8 (1 byte, direct)
     * 8-bit unsigned integer (0-255)
     *
     * @param value Value to encode (treated as unsigned)
     * @return Encoded byte array (1 byte)
     */
    public static byte[] encodeUint8(int value) {
        return new byte[]{ (byte) (value & 0xFF) };
    }


}  // class Encoder
