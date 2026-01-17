package protocol;

/**
 * Decoder.java
 *
 * AUTO-GENERATED - DO NOT EDIT
 */

// Binary Decoder - 8-bit binary


public final class Decoder {

    /**
     * Decode bool (1 byte)
     * Boolean value (true/false) (1 byte)
     */
    public static boolean decodeBool(byte[] buffer, int offset) {
        return buffer[offset] != 0;
    }

    /**
     * Decode float32 (4 bytes)
     * 32-bit IEEE 754 floating point (4 bytes, IEEE 754 little-endian)
     */
    public static float decodeFloat32(byte[] buffer, int offset) {
        int bits = (buffer[offset + 0] & 0xFF)
            | ((buffer[offset + 1] & 0xFF) << 8)
            | ((buffer[offset + 2] & 0xFF) << 16)
            | ((buffer[offset + 3] & 0xFF) << 24);
        return Float.intBitsToFloat(bits);
    }

    /**
     * Decode int16 (2 bytes)
     * 16-bit signed integer (-32768 to 32767) (2 bytes, little-endian)
     */
    public static short decodeInt16(byte[] buffer, int offset) {
        short result = (short)((buffer[offset + 0] & 0xFF) | ((buffer[offset + 1] & 0xFF) << 8));
        return (short) result;
    }

    /**
     * Decode int32 (4 bytes)
     * 32-bit signed integer (-2147483648 to 2147483647) (4 bytes, little-endian)
     */
    public static int decodeInt32(byte[] buffer, int offset) {
        int result = (buffer[offset + 0] & 0xFF)
            | ((buffer[offset + 1] & 0xFF) << 8)
            | ((buffer[offset + 2] & 0xFF) << 16)
            | ((buffer[offset + 3] & 0xFF) << 24);
        return (int) result;
    }

    /**
     * Decode int8 (1 byte)
     * 8-bit signed integer (-128 to 127) (1 byte, direct)
     */
    public static byte decodeInt8(byte[] buffer, int offset) {
        byte result = (byte)(buffer[offset + 0] & 0xFF);
        return result;
    }

    /**
     * Decode norm16 (2 bytes)
     * Normalized float (0.0-1.0) stored as uint16 for efficiency (2 bytes, little-endian (0-65535))
     */
    public static float decodeNorm16(byte[] buffer, int offset) {
        int raw = (buffer[offset + 0] & 0xFF)
            | ((buffer[offset + 1] & 0xFF) << 8);
        return (float) raw / 65535.0f;
    }

    /**
     * Decode norm8 (1 byte)
     * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth (1 byte, full 8-bit range (0-255))
     */
    public static float decodeNorm8(byte[] buffer, int offset) {
        int raw = buffer[offset] & 0xFF;
        return (float) raw / 255.0f;
    }

    /**
     * Decode string (variable length)
     * Variable-length UTF-8 string (prefixed with uint8 length) (1 byte length prefix + data (max 255 chars))
     *
     * Format: [length] [char0] [char1] ... [charN-1]
     * Max length: 255 chars
     * @param buffer Input buffer
     * @param offset Offset in buffer
     * @param maxLength Maximum string length (for validation)
     * @return Decoded string
     */
    public static String decodeString(byte[] buffer, int offset, int maxLength) {
        int len = Math.min(buffer[offset] & 0xFF, maxLength);
        StringBuilder sb = new StringBuilder(len);
        for (int i = 0; i < len; i++) {
            sb.append((char) (buffer[offset + 1 + i] & 0xFF));
        }
        return sb.toString();
    }

    /**
     * Decode uint16 (2 bytes)
     * 16-bit unsigned integer (0-65535) (2 bytes, little-endian)
     */
    public static int decodeUint16(byte[] buffer, int offset) {
        int result = (buffer[offset + 0] & 0xFF)
            | ((buffer[offset + 1] & 0xFF) << 8);
        return result;
    }

    /**
     * Decode uint32 (4 bytes)
     * 32-bit unsigned integer (0-4294967295) (4 bytes, little-endian)
     */
    public static long decodeUint32(byte[] buffer, int offset) {
        long result = (buffer[offset + 0] & 0xFF)
            | ((buffer[offset + 1] & 0xFF) << 8)
            | ((buffer[offset + 2] & 0xFF) << 16)
            | ((buffer[offset + 3] & 0xFF) << 24);
        return result;
    }

    /**
     * Decode uint8 (1 byte)
     * 8-bit unsigned integer (0-255) (1 byte, direct)
     */
    public static int decodeUint8(byte[] buffer, int offset) {
        int result = (buffer[offset + 0] & 0xFF);
        return result;
    }
}