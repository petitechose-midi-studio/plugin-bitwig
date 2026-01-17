package protocol;

/**
 * Encoder.java
 *
 * AUTO-GENERATED - DO NOT EDIT
 */

// Binary Encoder - 8-bit binary


public final class Encoder {

    /**
     * Encode bool (1 byte)
     * Boolean value (true/false) (0x00 or 0x01)
     * @return number of bytes written
     */
    public static int encodeBool(byte[] buffer, int offset, boolean val) {
        buffer[offset + 0] = (byte)(val ? 0x01 : 0x00);
        return 1;
    }

    /**
     * Encode float32 (4 bytes)
     * 32-bit IEEE 754 floating point (4 bytes, IEEE 754 little-endian)
     * @return number of bytes written
     */
    public static int encodeFloat32(byte[] buffer, int offset, float val) {
        int bits = Float.floatToIntBits(val);
        buffer[offset + 0] = (byte)(bits & 0xFF);
        buffer[offset + 1] = (byte)((bits >> 8) & 0xFF);
        buffer[offset + 2] = (byte)((bits >> 16) & 0xFF);
        buffer[offset + 3] = (byte)((bits >> 24) & 0xFF);
        return 4;
    }

    /**
     * Encode int16 (2 bytes)
     * 16-bit signed integer (-32768 to 32767) (2 bytes, little-endian)
     * @return number of bytes written
     */
    public static int encodeInt16(byte[] buffer, int offset, short value) {
        int val = value & 0xFFFF;
        buffer[offset + 0] = (byte)(val & 0xFF);
        buffer[offset + 1] = (byte)((val >> 8) & 0xFF);
        return 2;
    }

    /**
     * Encode int32 (4 bytes)
     * 32-bit signed integer (-2147483648 to 2147483647) (4 bytes, little-endian)
     * @return number of bytes written
     */
    public static int encodeInt32(byte[] buffer, int offset, int value) {
        int val = value & 0xFFFF;
        buffer[offset + 0] = (byte)(val & 0xFF);
        buffer[offset + 1] = (byte)((val >> 8) & 0xFF);
        buffer[offset + 2] = (byte)((val >> 16) & 0xFF);
        buffer[offset + 3] = (byte)((val >> 24) & 0xFF);
        return 4;
    }

    /**
     * Encode int8 (1 byte)
     * 8-bit signed integer (-128 to 127) (1 byte, direct)
     * @return number of bytes written
     */
    public static int encodeInt8(byte[] buffer, int offset, byte val) {
        buffer[offset + 0] = (byte)(val & 0xFF);
        return 1;
    }

    /**
     * Encode norm16 (2 bytes)
     * Normalized float (0.0-1.0) stored as uint16 for efficiency (2 bytes, little-endian (0-65535))
     * @return number of bytes written
     */
    public static int encodeNorm16(byte[] buffer, int offset, float val) {
        if (val < 0.0f) val = 0.0f;
        if (val > 1.0f) val = 1.0f;
        int norm = (int)(val * 65535.0f + 0.5f);
        buffer[offset + 0] = (byte)(norm & 0xFF);
        buffer[offset + 1] = (byte)((norm >> 8) & 0xFF);
        return 2;
    }

    /**
     * Encode norm8 (1 byte)
     * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth (1 byte, full 8-bit range (0-255))
     * @return number of bytes written
     */
    public static int encodeNorm8(byte[] buffer, int offset, float val) {
        if (val < 0.0f) val = 0.0f;
        if (val > 1.0f) val = 1.0f;
        int norm = (int)(val * 255.0f + 0.5f);
        buffer[offset + 0] = (byte)(norm & 0xFF);
        return 1;
    }

    /**
     * Encode string (variable length)
     * Variable-length UTF-8 string (prefixed with uint8 length) (1 byte length prefix + data (max 255 chars))
     *
     * Format: [length] [char0] [char1] ... [charN-1]
     * Max length: 255 chars
     * @return number of bytes written
     */
    public static int encodeString(byte[] buffer, int offset, String str) {
        int len = Math.min(str.length(), 255) & 0xFF;
        buffer[offset] = (byte)len;

        for (int i = 0; i < len; i++) {
            buffer[offset + 1 + i] = (byte)(str.charAt(i) & 0xFF);
        }
        return 1 + len;
    }

    /**
     * Encode uint16 (2 bytes)
     * 16-bit unsigned integer (0-65535) (2 bytes, little-endian)
     * @return number of bytes written
     */
    public static int encodeUint16(byte[] buffer, int offset, int val) {
        buffer[offset + 0] = (byte)(val & 0xFF);
        buffer[offset + 1] = (byte)((val >> 8) & 0xFF);
        return 2;
    }

    /**
     * Encode uint32 (4 bytes)
     * 32-bit unsigned integer (0-4294967295) (4 bytes, little-endian)
     * @return number of bytes written
     */
    public static int encodeUint32(byte[] buffer, int offset, long val) {
        buffer[offset + 0] = (byte)(val & 0xFF);
        buffer[offset + 1] = (byte)((val >> 8) & 0xFF);
        buffer[offset + 2] = (byte)((val >> 16) & 0xFF);
        buffer[offset + 3] = (byte)((val >> 24) & 0xFF);
        return 4;
    }

    /**
     * Encode uint8 (1 byte)
     * 8-bit unsigned integer (0-255) (1 byte, direct)
     * @return number of bytes written
     */
    public static int encodeUint8(byte[] buffer, int offset, int val) {
        buffer[offset + 0] = (byte)(val & 0xFF);
        return 1;
    }
}