package protocol;

import com.bitwig.extension.controller.api.ControllerHost;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * TcpTransport - TCP transport implementation
 *
 * Connection-oriented TCP communication with oc-bridge.
 * Uses length-prefixed framing: [length:4 bytes BE (u32)][payload]
 */
public class TcpTransport implements ProtocolTransport {

    private final ControllerHost host;
    private Socket socket;
    private DataInputStream input;
    private DataOutputStream output;
    private final AtomicBoolean isActive = new AtomicBoolean(true);
    private Thread receiverThread;

    public TcpTransport(ControllerHost host, String bridgeHost, int bridgePort, ReceiveCallback callback) {
        this.host = host;

        try {
            this.socket = new Socket(bridgeHost, bridgePort);
            this.socket.setTcpNoDelay(true);  // Disable Nagle for low latency
            this.input = new DataInputStream(socket.getInputStream());
            this.output = new DataOutputStream(socket.getOutputStream());

            // Start receiver thread
            startReceiverThread(callback);

            host.println("[TCP Transport] Connected to " + bridgeHost + ":" + bridgePort);
        } catch (Exception e) {
            host.errorln("[TCP Transport] Failed to connect: " + e.getMessage());
            throw new RuntimeException("Failed to start TCP transport", e);
        }
    }

    private void startReceiverThread(ReceiveCallback callback) {
        receiverThread = new Thread(() -> {
            while (isActive.get() && !socket.isClosed()) {
                try {
                    // Read length prefix (4 bytes, big-endian, unsigned)
                    int length = input.readInt();
                    if (length <= 0 || length > 4096) {
                        host.errorln("[TCP Transport] Invalid frame length: " + length);
                        continue;
                    }

                    // Read payload
                    byte[] data = new byte[length];
                    input.readFully(data);

                    callback.onReceive(data);

                } catch (SocketException e) {
                    if (isActive.get()) {
                        host.errorln("[TCP Transport] Socket error: " + e.getMessage());
                    }
                    break;
                } catch (IOException e) {
                    if (isActive.get()) {
                        host.errorln("[TCP Transport] Receive error: " + e.getMessage());
                    }
                    break;
                }
            }
        }, "TCP-Transport-Receiver");

        receiverThread.setDaemon(true);
        receiverThread.start();
    }

    @Override
    public synchronized void send(byte[] data) {
        if (!isActive.get() || socket.isClosed()) return;

        try {
            // Write length prefix (4 bytes, big-endian)
            output.writeInt(data.length);
            // Write payload
            output.write(data);
            output.flush();
        } catch (IOException e) {
            host.errorln("[TCP Transport] Send error: " + e.getMessage());
        }
    }

    @Override
    public void close() {
        isActive.set(false);
        try {
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
        } catch (IOException e) {
            // Ignore
        }
        if (receiverThread != null) {
            receiverThread.interrupt();
        }
    }

    @Override
    public boolean isConnected() {
        return socket != null && socket.isConnected() && !socket.isClosed() && isActive.get();
    }

    @Override
    public Type getType() {
        return Type.TCP;
    }
}
