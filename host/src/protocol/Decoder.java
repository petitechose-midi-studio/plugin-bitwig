package protocol;

/**
 * Decoder - 8-bit Binary Decoder (Serial8 Protocol)
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This class provides static decode methods for all builtin primitive
 * types. Decodes 8-bit binary bytes to native types.
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, norm8, norm16, string
 *
 * Decoding Strategy:
 * - Direct byte reads (no 7-bit constraint)
 * - Little-endian for multi-byte integers
 * - IEEE 754 for floats
 * - 8-bit length prefix for strings/arrays
 *
 * Performance:
 * - Static methods for zero overhead
 * - Direct byte array manipulation
 * - More efficient than 7-bit decoding
 *
 * Companion file: Encoder.java
 */
public final class Decoder {

    // Private constructor prevents instantiation (utility class)
    private Decoder() {
        throw new AssertionError("Utility class cannot be instantiated");
    }

    // ============================================================================
    // DECODE METHODS (8-bit binary bytes -> Type)
    // ============================================================================
    // Returns decoded value or throws IllegalArgumentException on error


    /**
     * Decode bool (1 byte)
     * Boolean value (true/false)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value
     * @throws IllegalArgumentException if insufficient data
     */
    public static boolean decodeBool(byte[] data, int offset) {
        if (data.length - offset < 1) {
            throw new IllegalArgumentException("Insufficient data for bool decode");
        }
        return data[offset] != 0x00;
    }


    /**
     * Decode float32 (4 bytes, IEEE 754 little-endian)
     * 32-bit IEEE 754 floating point
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value
     * @throws IllegalArgumentException if insufficient data
     */
    public static float decodeFloat32(byte[] data, int offset) {
        if (data.length - offset < 4) {
            throw new IllegalArgumentException("Insufficient data for float32 decode");
        }

        int bits = (data[offset] & 0xFF)
                 | ((data[offset + 1] & 0xFF) << 8)
                 | ((data[offset + 2] & 0xFF) << 16)
                 | ((data[offset + 3] & 0xFF) << 24);

        return Float.intBitsToFloat(bits);
    }


    /**
     * Decode int16 (2 bytes, little-endian)
     * 16-bit signed integer (-32768 to 32767)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value
     * @throws IllegalArgumentException if insufficient data
     */
    public static short decodeInt16(byte[] data, int offset) {
        if (data.length - offset < 2) {
            throw new IllegalArgumentException("Insufficient data for int16 decode");
        }

        int bits = (data[offset] & 0xFF)
                 | ((data[offset + 1] & 0xFF) << 8);

        return (short) bits;
    }


    /**
     * Decode int32 (4 bytes, little-endian)
     * 32-bit signed integer (-2147483648 to 2147483647)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value
     * @throws IllegalArgumentException if insufficient data
     */
    public static int decodeInt32(byte[] data, int offset) {
        if (data.length - offset < 4) {
            throw new IllegalArgumentException("Insufficient data for int32 decode");
        }

        return (data[offset] & 0xFF)
             | ((data[offset + 1] & 0xFF) << 8)
             | ((data[offset + 2] & 0xFF) << 16)
             | ((data[offset + 3] & 0xFF) << 24);
    }


    /**
     * Decode int8 (1 byte, direct)
     * 8-bit signed integer (-128 to 127)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value
     * @throws IllegalArgumentException if insufficient data
     */
    public static byte decodeInt8(byte[] data, int offset) {
        if (data.length - offset < 1) {
            throw new IllegalArgumentException("Insufficient data for int8 decode");
        }
        return data[offset];
    }


    /**
     * Decode norm16 (2 bytes -> float 0.0-1.0)
     * Normalized float (0.0-1.0) stored as uint16 for efficiency
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded normalized float value (0.0-1.0)
     * @throws IllegalArgumentException if insufficient data
     */
    public static float decodeNorm16(byte[] data, int offset) {
        if (data.length - offset < 2) {
            throw new IllegalArgumentException("Insufficient data for norm16 decode");
        }

        int val = (data[offset] & 0xFF)
                | ((data[offset + 1] & 0xFF) << 8);

        return val / 65535.0f;
    }


    /**
     * Decode norm8 (1 byte -> float 0.0-1.0)
     * Normalized float (0.0-1.0) stored as 7-bit uint8 for minimal bandwidth
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded normalized float value (0.0-1.0)
     * @throws IllegalArgumentException if insufficient data
     */
    public static float decodeNorm8(byte[] data, int offset) {
        if (data.length - offset < 1) {
            throw new IllegalArgumentException("Insufficient data for norm8 decode");
        }

        int val = data[offset] & 0xFF;
        return val / 255.0f;
    }


    /**
     * Decode string (variable length)
     * Variable-length UTF-8 string (prefixed with uint8 length)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @param maxLength Maximum allowed string length
     * @return Decoded string
     * @throws IllegalArgumentException if insufficient data or string too long
     */
    public static String decodeString(byte[] data, int offset, int maxLength) {
        if (data.length - offset < 1) {
            throw new IllegalArgumentException("Insufficient data for string decode");
        }

        int len = data[offset] & 0xFF;
        offset++;

        if (data.length - offset < len) {
            throw new IllegalArgumentException("Insufficient data for string content");
        }

        if (len > maxLength) {
            throw new IllegalArgumentException(
                "String length " + len + " exceeds maximum " + maxLength);
        }

        return new String(data, offset, len);
    }


    /**
     * Decode uint16 (2 bytes, little-endian)
     * 16-bit unsigned integer (0-65535)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value (unsigned, returned as int)
     * @throws IllegalArgumentException if insufficient data
     */
    public static int decodeUint16(byte[] data, int offset) {
        if (data.length - offset < 2) {
            throw new IllegalArgumentException("Insufficient data for uint16 decode");
        }

        return (data[offset] & 0xFF)
                | ((data[offset + 1] & 0xFF) << 8);
    }


    /**
     * Decode uint32 (4 bytes, little-endian)
     * 32-bit unsigned integer (0-4294967295)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value (unsigned, returned as long)
     * @throws IllegalArgumentException if insufficient data
     */
    public static long decodeUint32(byte[] data, int offset) {
        if (data.length - offset < 4) {
            throw new IllegalArgumentException("Insufficient data for uint32 decode");
        }

        long val = (data[offset] & 0xFFL)
                 | ((data[offset + 1] & 0xFFL) << 8)
                 | ((data[offset + 2] & 0xFFL) << 16)
                 | ((data[offset + 3] & 0xFFL) << 24);

        return (int) val;
    }


    /**
     * Decode uint8 (1 byte, direct)
     * 8-bit unsigned integer (0-255)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value (unsigned, returned as int)
     * @throws IllegalArgumentException if insufficient data
     */
    public static int decodeUint8(byte[] data, int offset) {
        if (data.length - offset < 1) {
            throw new IllegalArgumentException("Insufficient data for uint8 decode");
        }
        return (data[offset] & 0xFF);
    }


}  // class Decoder
