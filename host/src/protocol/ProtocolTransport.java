package protocol;

/**
 * ProtocolTransport - Transport layer interface for protocol communication
 *
 * Abstracts the underlying transport (UDP or TCP) from the protocol logic.
 */
public interface ProtocolTransport {

    /**
     * Transport type enum
     */
    enum Type {
        UDP,
        TCP
    }

    /**
     * Callback for received data
     */
    @FunctionalInterface
    interface ReceiveCallback {
        void onReceive(byte[] data);
    }

    /**
     * Send data to the bridge
     *
     * @param data The data to send
     */
    void send(byte[] data);

    /**
     * Send data to the bridge with offset and length (zero-copy)
     *
     * @param buffer The buffer containing data
     * @param offset Start offset in buffer
     * @param length Number of bytes to send
     */
    default void send(byte[] buffer, int offset, int length) {
        // Default implementation copies - transports can override for zero-copy
        byte[] data = new byte[length];
        System.arraycopy(buffer, offset, data, 0, length);
        send(data);
    }

    /**
     * Close the transport
     */
    void close();

    /**
     * Check if transport is connected/active
     */
    boolean isConnected();

    /**
     * Get transport type
     */
    Type getType();
}
