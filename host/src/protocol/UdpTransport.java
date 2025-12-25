package protocol;

import com.bitwig.extension.controller.api.ControllerHost;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * UdpTransport - UDP transport implementation
 *
 * Connectionless UDP communication with oc-bridge.
 * Each datagram is one complete message.
 */
public class UdpTransport implements ProtocolTransport {

    private static final int MAX_PACKET_SIZE = 4096;

    private final ControllerHost host;
    private final DatagramSocket socket;
    private final InetAddress bridgeAddress;
    private final int bridgePort;
    private final AtomicBoolean isActive = new AtomicBoolean(true);
    private Thread receiverThread;

    public UdpTransport(ControllerHost host, String bridgeHost, int bridgePort, ReceiveCallback callback) {
        this.host = host;
        this.bridgePort = bridgePort;

        try {
            this.bridgeAddress = InetAddress.getByName(bridgeHost);
            this.socket = new DatagramSocket();

            // Start receiver thread
            startReceiverThread(callback);

            // Send registration packet
            sendRegistration();

            // Startup log disabled
        } catch (Exception e) {
            host.errorln("[UDP Transport] Failed to start: " + e.getMessage());
            throw new RuntimeException("Failed to start UDP transport", e);
        }
    }

    private void sendRegistration() {
        try {
            byte[] empty = new byte[]{0};
            DatagramPacket packet = new DatagramPacket(empty, empty.length, bridgeAddress, bridgePort);
            socket.send(packet);
        } catch (IOException e) {
            host.errorln("[UDP Transport] Registration failed: " + e.getMessage());
        }
    }

    private void startReceiverThread(ReceiveCallback callback) {
        receiverThread = new Thread(() -> {
            byte[] buffer = new byte[MAX_PACKET_SIZE];
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

            while (isActive.get() && !socket.isClosed()) {
                try {
                    socket.receive(packet);

                    byte[] data = new byte[packet.getLength()];
                    System.arraycopy(packet.getData(), 0, data, 0, packet.getLength());

                    callback.onReceive(data);

                } catch (SocketException e) {
                    if (isActive.get()) {
                        host.errorln("[UDP Transport] Socket error: " + e.getMessage());
                    }
                    break;
                } catch (IOException e) {
                    if (isActive.get()) {
                        host.errorln("[UDP Transport] Receive error: " + e.getMessage());
                    }
                }
            }
        }, "UDP-Transport-Receiver");

        receiverThread.setDaemon(true);
        receiverThread.start();
    }

    @Override
    public void send(byte[] data) {
        send(data, 0, data.length);
    }

    @Override
    public void send(byte[] buffer, int offset, int length) {
        if (!isActive.get() || socket.isClosed()) return;

        try {
            // Zero-copy: DatagramPacket uses offset/length directly
            DatagramPacket packet = new DatagramPacket(buffer, offset, length, bridgeAddress, bridgePort);
            socket.send(packet);
        } catch (IOException e) {
            host.errorln("[UDP Transport] Send error: " + e.getMessage());
        }
    }

    @Override
    public void close() {
        isActive.set(false);
        if (socket != null && !socket.isClosed()) {
            socket.close();
        }
        if (receiverThread != null) {
            receiverThread.interrupt();
        }
    }

    @Override
    public boolean isConnected() {
        return socket != null && !socket.isClosed() && isActive.get();
    }

    @Override
    public Type getType() {
        return Type.UDP;
    }
}
