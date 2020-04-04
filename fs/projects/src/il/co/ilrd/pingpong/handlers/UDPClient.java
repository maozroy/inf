package il.co.ilrd.pingpong.handlers;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;


public class UDPClient {

public class UdpMessageClient {
	private boolean isRunning = true;
	private long id = Thread.currentThread().getId();

	public void startClient() throws IOException, IOException, InterruptedException, ClassNotFoundException {
		int portNumber = 5000;

		System.out.println("Connecting udp to " + InetAddress.getLocalHost() + " in port " + portNumber);

		byte[] reqBuf = new byte[256];
		DatagramPacket packet = new DatagramPacket(reqBuf, reqBuf.length, InetAddress.getByName("255.255.255.255"), portNumber);

		try (DatagramSocket datagramSocket = new DatagramSocket();) {
//			InputDetector exitDetector = new InputDetector(() -> stopUdpClient(datagramSocket), "exit");
//			exitDetector.start();
			while (isRunning) {
				Thread.sleep(1000);
				sendData(datagramSocket, packet);
				datagramSocket.receive(packet);
				printReceivedData(packet);
			}
		} catch (SocketException e) {
			System.err.println("closing udp client");

		}
	}

	private void sendData(DatagramSocket datagramSocket, DatagramPacket packet) throws IOException {
		ServerMessage message = new ServerMessage(ProtocolIndex.PINGPONG, new PingPongMessage("pong"));
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		ObjectOutputStream out = new ObjectOutputStream(bos);
		out.writeObject(message);
		out.flush();
		byte[] reqBuf = bos.toByteArray();
//		byte[] reqBuf = new byte[256];
		// reqBuf = ("udp pong " + id).getBytes();
		packet.setData(reqBuf);
		datagramSocket.send(packet);
	}

	private void stopUdpClient(DatagramSocket datagramSocket) {
		isRunning = false;
		datagramSocket.close();
	}

	private void printReceivedData(DatagramPacket packet) throws ClassNotFoundException, IOException {
        byte[] data = packet.getData();
        PingPongMessage message = (PingPongMessage) PingPongMessage.toObject(data);

    	System.out.println(message);
		System.out.println("Client channel UDP " + id + " received: " + message.getKey());
	}
}
	public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {

		new UDPClient().new UdpMessageClient().startClient();

	}
}


