package protocol;

/**
 * Encoder - 8-bit Binary Streaming Encoder (Serial8 Protocol)
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This class provides static streaming write methods for all builtin primitive
 * types. Uses full 8-bit byte range for efficient zero-allocation encoding.
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, norm8, norm16, string
 *
 * Encoding Strategy:
 * - Streaming pattern: writeXxx(buffer, offset, value) returns bytes written
 * - Direct byte writes to provided buffer (zero allocation)
 * - Little-endian for multi-byte integers
 * - IEEE 754 for floats
 * - 8-bit length prefix for strings/arrays
 *
 * Performance:
 * - Zero allocation per encode call
 * - Direct buffer manipulation
 * - Compiler-optimized inline writes
 */
public final class Encoder {

    // Private constructor prevents instantiation (utility class)
    private Encoder() {
        throw new AssertionError("Utility class cannot be instantiated");
    }

    // ============================================================================
    // STREAMING WRITE METHODS (Type -> buffer, returns bytes written)
    // ============================================================================


    /**
     * Write bool (1 byte: 0x00 or 0x01)
     * Boolean value (true/false)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Value to encode
     * @return Number of bytes written (1)
     */
    public static int writeBool(byte[] buffer, int offset, boolean value) {
        buffer[offset] = (byte) (value ? 0x01 : 0x00);
        return 1;
    }


    /**
     * Write float32 (4 bytes, IEEE 754 little-endian)
     * 32-bit IEEE 754 floating point
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Value to encode
     * @return Number of bytes written (4)
     */
    public static int writeFloat32(byte[] buffer, int offset, float value) {
        int bits = Float.floatToRawIntBits(value);
        buffer[offset] = (byte) (bits & 0xFF);
        buffer[offset + 1] = (byte) ((bits >> 8) & 0xFF);
        buffer[offset + 2] = (byte) ((bits >> 16) & 0xFF);
        buffer[offset + 3] = (byte) ((bits >> 24) & 0xFF);
        return 4;
    }


    /**
     * Write int16 (2 bytes, little-endian)
     * 16-bit signed integer (-32768 to 32767)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Value to encode
     * @return Number of bytes written (2)
     */
    public static int writeInt16(byte[] buffer, int offset, short value) {
        int bits = value & 0xFFFF;
        buffer[offset] = (byte) (bits & 0xFF);
        buffer[offset + 1] = (byte) ((bits >> 8) & 0xFF);
        return 2;
    }


    /**
     * Write int32 (4 bytes, little-endian)
     * 32-bit signed integer (-2147483648 to 2147483647)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Value to encode
     * @return Number of bytes written (4)
     */
    public static int writeInt32(byte[] buffer, int offset, int value) {
        buffer[offset] = (byte) (value & 0xFF);
        buffer[offset + 1] = (byte) ((value >> 8) & 0xFF);
        buffer[offset + 2] = (byte) ((value >> 16) & 0xFF);
        buffer[offset + 3] = (byte) ((value >> 24) & 0xFF);
        return 4;
    }


    /**
     * Write int8 (1 byte, direct)
     * 8-bit signed integer (-128 to 127)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Value to encode
     * @return Number of bytes written (1)
     */
    public static int writeInt8(byte[] buffer, int offset, byte value) {
        buffer[offset] = value;
        return 1;
    }


    /**
     * Write norm16 (2 bytes, little-endian)
     * Normalized float (0.0-1.0) stored as uint16 for efficiency
     *
     * Converts float 0.0-1.0 to uint16 0-65535 for high precision.
     * Precision: ~0.0015% (1/65535)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Float value to encode (clamped to 0.0-1.0)
     * @return Number of bytes written (2)
     */
    public static int writeNorm16(byte[] buffer, int offset, float value) {
        float clamped = Math.max(0.0f, Math.min(1.0f, value));
        int val = Math.round(clamped * 65535.0f) & 0xFFFF;
        buffer[offset] = (byte) (val & 0xFF);
        buffer[offset + 1] = (byte) ((val >> 8) & 0xFF);
        return 2;
    }


    /**
     * Write norm8 (1 byte, full 8-bit range)
     * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth
     *
     * Converts float 0.0-1.0 to 8-bit value 0-255 for optimal precision.
     * Precision: ~0.4% (1/255), better than 7-bit norm8.
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Float value to encode (clamped to 0.0-1.0)
     * @return Number of bytes written (1)
     */
    public static int writeNorm8(byte[] buffer, int offset, float value) {
        float clamped = Math.max(0.0f, Math.min(1.0f, value));
        buffer[offset] = (byte) (Math.round(clamped * 255.0f) & 0xFF);
        return 1;
    }


    /**
     * Write string (variable length: 1 byte length + data)
     * Variable-length UTF-8 string (prefixed with uint8 length)
     *
     * Format: [length (8-bit)] [char0] [char1] ... [charN-1]
     * Max length: 255 chars (8-bit length encoding)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value String to encode
     * @param maxLength Maximum allowed string length
     * @return Number of bytes written (1 + string.length)
     */
    public static int writeString(byte[] buffer, int offset, String value, int maxLength) {
        if (value == null) {
            value = "";
        }

        int len = Math.min(value.length(), Math.min(maxLength, 255));
        buffer[offset] = (byte) len;

        for (int i = 0; i < len; i++) {
            buffer[offset + 1 + i] = (byte) value.charAt(i);
        }

        return 1 + len;
    }


    /**
     * Write uint16 (2 bytes, little-endian)
     * 16-bit unsigned integer (0-65535)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Value to encode (treated as unsigned)
     * @return Number of bytes written (2)
     */
    public static int writeUint16(byte[] buffer, int offset, int value) {
        int val = value & 0xFFFF;
        buffer[offset] = (byte) (val & 0xFF);
        buffer[offset + 1] = (byte) ((val >> 8) & 0xFF);
        return 2;
    }


    /**
     * Write uint32 (4 bytes, little-endian)
     * 32-bit unsigned integer (0-4294967295)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Value to encode (treated as unsigned)
     * @return Number of bytes written (4)
     */
    public static int writeUint32(byte[] buffer, int offset, long value) {
        long val = value & 0xFFFFFFFFL;
        buffer[offset] = (byte) (val & 0xFF);
        buffer[offset + 1] = (byte) ((val >> 8) & 0xFF);
        buffer[offset + 2] = (byte) ((val >> 16) & 0xFF);
        buffer[offset + 3] = (byte) ((val >> 24) & 0xFF);
        return 4;
    }


    /**
     * Write uint8 (1 byte, direct)
     * 8-bit unsigned integer (0-255)
     *
     * @param buffer Output buffer
     * @param offset Write position in buffer
     * @param value Value to encode (treated as unsigned)
     * @return Number of bytes written (1)
     */
    public static int writeUint8(byte[] buffer, int offset, int value) {
        buffer[offset] = (byte) (value & 0xFF);
        return 1;
    }


}  // class Encoder
