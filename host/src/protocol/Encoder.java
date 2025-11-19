package protocol;

/**
 * Encoder - 7-bit MIDI-safe Encoder/Decoder
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This class provides static encode/decode methods for all builtin primitive
 * types. All multi-byte types use 7-bit encoding to ensure MIDI-safe
 * transmission (all bytes < 0x80).
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, string
 *
 * Encoding Strategy:
 * - Single-byte types (uint8, int8): No encoding (already < 0x80 when valid)
 * - Multi-byte integers: 7-bit per byte (e.g., uint16: 2→3 bytes)
 * - Float32: 4→5 bytes (7-bit chunks)
 * - String: length prefix (7-bit) + ASCII data
 *
 * Performance:
 * - Static methods for zero overhead
 * - Direct byte array manipulation
 * - Optimized for protocol efficiency
 */
public final class Encoder {

    // Private constructor prevents instantiation (utility class)
    private Encoder() {
        throw new AssertionError("Utility class cannot be instantiated");
    }

    // ============================================================================
    // ENCODE METHODS (Type → 7-bit MIDI-safe bytes)
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
     * Encode float32 (4 bytes → 5 bytes, 7-bit encoding)
     * 32-bit IEEE 754 floating point
     * Overhead: +25% (4→5 bytes)
     *
     * Uses IEEE 754 bit representation, split into 7-bit chunks.
     *
     * @param value Value to encode
     * @return Encoded byte array (5 bytes)
     */
    public static byte[] encodeFloat32(float value) {
        int bits = Float.floatToRawIntBits(value);
        long unsignedBits = bits & 0xFFFFFFFFL;

        return new byte[]{
            (byte) (unsignedBits & 0x7F),           // bits 0-6
            (byte) ((unsignedBits >> 7) & 0x7F),    // bits 7-13
            (byte) ((unsignedBits >> 14) & 0x7F),   // bits 14-20
            (byte) ((unsignedBits >> 21) & 0x7F),   // bits 21-27
            (byte) ((unsignedBits >> 28) & 0x0F)    // bits 28-31
        };
    }


    /**
     * Encode int16 (2 bytes → 3 bytes, 7-bit encoding)
     * 16-bit signed integer (-32768 to 32767)
     * Overhead: +50% (2→3 bytes)
     *
     * @param value Value to encode
     * @return Encoded byte array (3 bytes)
     */
    public static byte[] encodeInt16(short value) {
        int bits = value & 0xFFFF;
        return new byte[]{
            (byte) (bits & 0x7F),
            (byte) ((bits >> 7) & 0x7F),
            (byte) ((bits >> 14) & 0x03)
        };
    }


    /**
     * Encode int32 (4 bytes → 5 bytes, 7-bit encoding)
     * 32-bit signed integer (-2147483648 to 2147483647)
     * Overhead: +25% (4→5 bytes)
     *
     * @param value Value to encode
     * @return Encoded byte array (5 bytes)
     */
    public static byte[] encodeInt32(int value) {
        long bits = value & 0xFFFFFFFFL;
        return new byte[]{
            (byte) (bits & 0x7F),
            (byte) ((bits >> 7) & 0x7F),
            (byte) ((bits >> 14) & 0x7F),
            (byte) ((bits >> 21) & 0x7F),
            (byte) ((bits >> 28) & 0x0F)
        };
    }


    /**
     * Encode int8 (1 byte, signed → unsigned mapping)
     * 8-bit signed integer (-128 to 127)
     *
     * @param value Value to encode
     * @return Encoded byte array (1 byte)
     */
    public static byte[] encodeInt8(byte value) {
        return new byte[]{ (byte) (value & 0x7F) };
    }


    /**
     * Encode string (variable length: 1 byte length + data)
     * Variable-length UTF-8 string (prefixed with uint8 length, max 16 chars)
     *
     * Format: [length (7-bit)] [char0] [char1] ... [charN-1]
     * Max length: 127 chars (7-bit length encoding)
     *
     * @param value String to encode
     * @param maxLength Maximum allowed string length
     * @return Encoded byte array (1 + string.length bytes)
     * @throws IllegalArgumentException if string exceeds maxLength
     */
    public static byte[] encodeString(String value, int maxLength) {
        if (value == null) {
            value = "";
        }

        int len = Math.min(value.length(), Math.min(maxLength, 127));
        byte[] result = new byte[1 + len];

        result[0] = (byte) (len & 0x7F);

        for (int i = 0; i < len; i++) {
            result[1 + i] = (byte) (value.charAt(i) & 0x7F);  // ASCII chars < 0x80
        }

        return result;
    }


    /**
     * Encode uint16 (2 bytes → 3 bytes, 7-bit encoding)
     * 16-bit unsigned integer (0-65535)
     * Overhead: +50% (2→3 bytes)
     *
     * @param value Value to encode (treated as unsigned)
     * @return Encoded byte array (3 bytes)
     */
    public static byte[] encodeUint16(int value) {
        int val = value & 0xFFFF;  // Treat as unsigned
        return new byte[]{
            (byte) (val & 0x7F),           // bits 0-6
            (byte) ((val >> 7) & 0x7F),    // bits 7-13
            (byte) ((val >> 14) & 0x03)    // bits 14-15 (only 2 bits needed)
        };
    }


    /**
     * Encode uint32 (4 bytes → 5 bytes, 7-bit encoding)
     * 32-bit unsigned integer (0-4294967295)
     * Overhead: +25% (4→5 bytes)
     *
     * @param value Value to encode (treated as unsigned)
     * @return Encoded byte array (5 bytes)
     */
    public static byte[] encodeUint32(long value) {
        long val = value & 0xFFFFFFFFL;  // Treat as unsigned
        return new byte[]{
            (byte) (val & 0x7F),           // bits 0-6
            (byte) ((val >> 7) & 0x7F),    // bits 7-13
            (byte) ((val >> 14) & 0x7F),   // bits 14-20
            (byte) ((val >> 21) & 0x7F),   // bits 21-27
            (byte) ((val >> 28) & 0x0F)    // bits 28-31 (only 4 bits needed)
        };
    }


    /**
     * Encode uint8 (1 byte, no transformation needed if < 0x80)
     * 8-bit unsigned integer (0-255)
     *
     * @param value Value to encode (treated as unsigned)
     * @return Encoded byte array (1 byte)
     */
    public static byte[] encodeUint8(int value) {
        return new byte[]{ (byte) (value & 0x7F) };
    }


}  // class Encoder
