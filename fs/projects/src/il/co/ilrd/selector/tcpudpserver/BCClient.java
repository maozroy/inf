package il.co.ilrd.selector.tcpudpserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class BCClient {
	private static final int PORT_NUMBER = 24817;
	private static final int BUFFER_SIZE = 256;

	public static void main(String[] args) {
		byte[] buffer = new byte[BUFFER_SIZE];
		InetAddress address = null;
		try {
			address = InetAddress.getByName("255.255.255.255");
		} catch (UnknownHostException e1) {
			e1.printStackTrace();
		}
		
		DatagramPacket packet = new DatagramPacket(buffer, buffer.length, address, PORT_NUMBER);

		try (DatagramSocket socket = new DatagramSocket();) {
			ExitClient exitThread = new ExitClient(socket);
			new Thread(exitThread).start();

			while (true) {
				String message = "ping";
				buffer = message.getBytes();
				packet.setData(buffer);
				socket.send(packet);
				socket.receive(packet);

				String messegeReceived = new String(packet.getData(), 0, packet.getLength());
				System.out.println("Broadcast Client received: " + messegeReceived);
			}
		} catch (SocketException e) {
			System.out.println(">>> Exits <<<");
			return;
		} catch (IOException e) {
			e.printStackTrace();
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

			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}