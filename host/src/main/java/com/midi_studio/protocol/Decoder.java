package com.midi_studio.protocol;

/**
 * Decoder - 7-bit MIDI-safe Decoder
 *
 * AUTO-GENERATED - DO NOT EDIT
 * Generated from: builtin_types.yaml
 *
 * This class provides static decode methods for all builtin primitive
 * types. Converts 7-bit MIDI-safe bytes back to native types.
 *
 * Supported types: bool, uint8, uint16, uint32, int8, int16, int32, float32, string
 *
 * Decoding Strategy:
 * - SysEx bytes (7-bit) → Native types
 * - Multi-byte integers: 7-bit chunks → full integers
 * - Float32: 5 bytes → 4 bytes (7-bit chunks → IEEE 754)
 * - String: length prefix + ASCII data
 *
 * Performance:
 * - Static methods for zero overhead
 * - Direct byte array manipulation
 * - Optimized for protocol efficiency
 *
 * Companion file: Encoder.java (Type → SysEx direction)
 */
public final class Decoder {

    // Private constructor prevents instantiation (utility class)
    private Decoder() {
        throw new AssertionError("Utility class cannot be instantiated");
    }

    // ============================================================================
    // DECODE METHODS (7-bit MIDI-safe bytes → Type)
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
     * Decode float32 (5 bytes → 4 bytes)
     * 32-bit IEEE 754 floating point
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value
     * @throws IllegalArgumentException if insufficient data
     */
    public static float decodeFloat32(byte[] data, int offset) {
        if (data.length - offset < 5) {
            throw new IllegalArgumentException("Insufficient data for float32 decode");
        }

        long bits = (data[offset] & 0x7FL)
                  | ((data[offset + 1] & 0x7FL) << 7)
                  | ((data[offset + 2] & 0x7FL) << 14)
                  | ((data[offset + 3] & 0x7FL) << 21)
                  | ((data[offset + 4] & 0x0FL) << 28);

        return Float.intBitsToFloat((int) bits);
    }


    /**
     * Decode int16 (3 bytes → 2 bytes)
     * 16-bit signed integer (-32768 to 32767)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value
     * @throws IllegalArgumentException if insufficient data
     */
    public static short decodeInt16(byte[] data, int offset) {
        if (data.length - offset < 3) {
            throw new IllegalArgumentException("Insufficient data for int16 decode");
        }

        int bits = (data[offset] & 0x7F)
                 | ((data[offset + 1] & 0x7F) << 7)
                 | ((data[offset + 2] & 0x03) << 14);

        return (short) bits;
    }


    /**
     * Decode int32 (5 bytes → 4 bytes)
     * 32-bit signed integer (-2147483648 to 2147483647)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value
     * @throws IllegalArgumentException if insufficient data
     */
    public static int decodeInt32(byte[] data, int offset) {
        if (data.length - offset < 5) {
            throw new IllegalArgumentException("Insufficient data for int32 decode");
        }

        long bits = (data[offset] & 0x7FL)
                  | ((data[offset + 1] & 0x7FL) << 7)
                  | ((data[offset + 2] & 0x7FL) << 14)
                  | ((data[offset + 3] & 0x7FL) << 21)
                  | ((data[offset + 4] & 0x0FL) << 28);

        return (int) bits;
    }


    /**
     * Decode int8 (1 byte)
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
        return (byte) (data[offset] & 0x7F);
    }


    /**
     * Decode string (variable length)
     * Variable-length UTF-8 string (prefixed with uint8 length, max 16 chars)
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

        int len = data[offset] & 0x7F;
        offset++;

        if (data.length - offset < len) {
            throw new IllegalArgumentException("Insufficient data for string content");
        }

        if (len > maxLength) {
            throw new IllegalArgumentException(
                "String length " + len + " exceeds maximum " + maxLength);
        }

        StringBuilder sb = new StringBuilder(len);
        for (int i = 0; i < len; i++) {
            sb.append((char) (data[offset + i] & 0x7F));
        }

        return sb.toString();
    }


    /**
     * Decode uint16 (3 bytes → 2 bytes)
     * 16-bit unsigned integer (0-65535)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value (unsigned, returned as int)
     * @throws IllegalArgumentException if insufficient data
     */
    public static int decodeUint16(byte[] data, int offset) {
        if (data.length - offset < 3) {
            throw new IllegalArgumentException("Insufficient data for uint16 decode");
        }

        return (data[offset] & 0x7F)
                | ((data[offset + 1] & 0x7F) << 7)
                | ((data[offset + 2] & 0x03) << 14);
    }


    /**
     * Decode uint32 (5 bytes → 4 bytes)
     * 32-bit unsigned integer (0-4294967295)
     *
     * @param data Byte array containing encoded data
     * @param offset Start offset in array
     * @return Decoded value (unsigned, returned as long)
     * @throws IllegalArgumentException if insufficient data
     */
    public static long decodeUint32(byte[] data, int offset) {
        if (data.length - offset < 5) {
            throw new IllegalArgumentException("Insufficient data for uint32 decode");
        }

        long val = (data[offset] & 0x7FL)
                 | ((data[offset + 1] & 0x7FL) << 7)
                 | ((data[offset + 2] & 0x7FL) << 14)
                 | ((data[offset + 3] & 0x7FL) << 21)
                 | ((data[offset + 4] & 0x0FL) << 28);

        return (int) val;
    }


    /**
     * Decode uint8 (1 byte)
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
        return (data[offset] & 0x7F);
    }


}  // class Decoder
