package il.co.ilrd.server.chatserver;

import java.io.*;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

import il.co.ilrd.server.general.ProtocolPort;
import il.co.ilrd.server.general.ProtocolType;
import il.co.ilrd.server.general.ServerMessage;


public class ChatClientEyal  {
	
	private String username;
	private String ipString;

	public ChatClientEyal(String username, String ipString) {
		this.username = username;
		this.ipString = ipString;
	}

	public void startClient() throws IOException, ClassNotFoundException, InterruptedException {

		int portNumber = ProtocolPort.CHAT_PROTOCOL_PORT.getPort();
		System.out.println(username + " Connecting to chat server " + ipString + " in port " + portNumber);
		ByteBuffer buffer = ByteBuffer.allocate(4096);
		SocketChannel clientSocket = null;

		ServerMessage registerMessage = new ServerMessage(ProtocolType.CHAT_SERVER,
				new ChatServerMessage(ChatProtocolKeys.REGISTRATION_REQUEST, username));
		ServerMessage receivedMssage = null;

		byte[] messageArray = registerMessage.toByteArray(registerMessage);
		try {
			clientSocket = SocketChannel.open(new InetSocketAddress(ipString, portNumber));
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		// send register message
		messageArray = registerMessage.toByteArray(registerMessage);
		buffer.clear();
		buffer.put(messageArray);
		buffer.flip();
		clientSocket.write(buffer);

		// receive registration result
		buffer.clear();
		clientSocket.read(buffer);
		receivedMssage = (ServerMessage) ServerMessage.toObject(buffer.array());
		System.out.println("response:" + receivedMssage.getData().getKey());
		System.out.println("response: " + receivedMssage.getData().getData());
		Thread.sleep(1000);
		new Thread(new ReceieveMessageFromServer(clientSocket)).start();
		startGettingInput(clientSocket);
	}

	private class ReceieveMessageFromServer implements Runnable {
		private SocketChannel clientSocket;
		private ByteBuffer buffer = ByteBuffer.allocate(4096);
		private ServerMessage receivedMssage = null;

		public ReceieveMessageFromServer(SocketChannel clientSocket) {
			this.clientSocket = clientSocket;
		}

		@Override
		public void run() {
			while (true) {
				buffer.clear();
				try {
					System.out.println("waiting on read");
					clientSocket.read(buffer);
					receivedMssage = (ServerMessage) ServerMessage.toObject(buffer.array());
				} catch (IOException | ClassNotFoundException e) {
					e.printStackTrace();
				}

//				System.out.println(receivedMssage.getData().getKey());
				System.out.println(receivedMssage.getData().getData());

			}
		}
	}

	private void startGettingInput(SocketChannel clientSocket) {
		try(Scanner sc = new Scanner(System.in)){
			ByteBuffer buffer = ByteBuffer.allocate(4096);

			String input = "";
			while (!input.equalsIgnoreCase("exit")) {
				System.out.print("Enter message: ");
				input = sc.nextLine();
				if (input.length() < 80) {
					sendMessage(input, buffer, clientSocket);
				}
				else {
					System.out.println("input cannot exced 80 chars");
				}
			}
		}
		
	}

	private void sendMessage(String input, ByteBuffer buffer, SocketChannel clientSocket) {
		ServerMessage orginalMessage = new ServerMessage(ProtocolType.CHAT_SERVER,
				new ChatServerMessage(ChatProtocolKeys.MESSAGE, input));
		byte[] messageArray;
		try {
			messageArray = orginalMessage.toByteArray(orginalMessage);
			buffer.clear();
			buffer.put(messageArray);
			buffer.flip();
		} catch (IOException e1) {
			e1.printStackTrace();
		}		

		try {
			clientSocket.write(buffer);
		} catch (IOException e) {
			e.printStackTrace();
		}		
	}

}