package il.co.ilrd.selector.tcpudpserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.SocketException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPClient {
	private static final int PORT_NUMBER = 24817;
	private static final int BUFFER_SIZE = 256;

	public static void main(String[] args) throws IOException, InterruptedException {
		byte[] buffer = new byte[BUFFER_SIZE];
		InetAddress address = InetAddress.getLocalHost();
		DatagramPacket packet = new DatagramPacket(buffer, buffer.length, address, PORT_NUMBER);

		try (DatagramSocket socket = new DatagramSocket()) {
			new Thread(new ExitClient(socket)).start();

			while (true) {
				sendData(socket, buffer, packet);
				recieveData(socket, packet);
			}
		} catch (SocketException e) {
			System.out.println(">>> Exits <<<");
			return;
		}
	}

	private static class ExitClient implements Runnable {
		DatagramSocket socket;

		public ExitClient(DatagramSocket socket) {
			this.socket = socket;
		}

		@Override
		public void run() {
			BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
			String input;

			try {
				input = reader.readLine();
				while (!input.equalsIgnoreCase("exit")) {
					input = reader.readLine();
				}

				socket.close();

			} catch (SocketException e) {
				return;
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private static void recieveData(DatagramSocket clientSocket, DatagramPacket packet) throws IOException {
		clientSocket.receive(packet);
		String messegeReceived = new String(packet.getData(), 0, packet.getLength());
		System.out.println("UDP Client received: " + messegeReceived);
	}

	private static void sendData(DatagramSocket clientSocket, byte[] buffer, DatagramPacket packet) throws IOException {
		String serverMessage = "ping";

		buffer = serverMessage.getBytes();
		packet.setData(buffer);
		clientSocket.send(packet);
	}
}