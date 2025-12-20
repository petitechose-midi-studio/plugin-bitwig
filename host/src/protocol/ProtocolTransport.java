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
