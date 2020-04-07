package il.co.ilrd.vpn.chatserver;

import java.io.*;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;


public class GlobalChatClient  {
	
	private String username;
	private String ip;

	public GlobalChatClient(String username, String ip) {
		this.username = username;
		this.ip = ip;
	}

	public void startClient() throws IOException, ClassNotFoundException, InterruptedException {

		String hostName = ip;
		int portNumber = GlobalProtocolPort.CHAT_PROTOCOL_PORT.getPort();
		System.out.println(username + " Connecting to chat server " + hostName + " in port " + portNumber);
		ByteBuffer buffer = ByteBuffer.allocate(4096);
		SocketChannel clientSocket = null;

		GlobalServerMessage registerMessage = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER,
				new GlobalChatServerMessage(GlobalChatProtocolKeys.REGISTRATION_REQUEST, username));
		GlobalServerMessage receivedMssage = null;

		byte[] messageArray =  ObjectCovertUtil.toByteArray(registerMessage);
		try {
			clientSocket = SocketChannel.open(new InetSocketAddress(hostName, portNumber));
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		// send register message
		messageArray =  ObjectCovertUtil.toByteArray(registerMessage);
		buffer.clear();
		buffer.put(messageArray);
		buffer.flip();
		clientSocket.write(buffer);

		// receive registration result
		buffer.clear();
		clientSocket.read(buffer);
		receivedMssage = (GlobalServerMessage) ObjectCovertUtil.toObject(buffer.array()); 
		System.out.println("response:" + receivedMssage.getData().getKey());
		System.out.println("response: " + receivedMssage.getData().getData());
		Thread.sleep(1000);
		new Thread(new ReceieveMessageFromServer(clientSocket)).start();
		startGettingInput(clientSocket);
	}

	private class ReceieveMessageFromServer implements Runnable {
		private SocketChannel clientSocket;
		private ByteBuffer buffer = ByteBuffer.allocate(4096);
		private GlobalServerMessage receivedMssage = null;

		public ReceieveMessageFromServer(SocketChannel clientSocket) {
			this.clientSocket = clientSocket;
		}

		@Override
		public void run() {
			while (true) {
				buffer.clear();
				try {
					clientSocket.read(buffer);
					receivedMssage = (GlobalServerMessage) ObjectCovertUtil.toObject(buffer.array());
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
		GlobalServerMessage orginalMessage = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER,
				new GlobalChatServerMessage(GlobalChatProtocolKeys.MESSAGE, input));
		byte[] messageArray;
		try {
			messageArray = ObjectCovertUtil.toByteArray(orginalMessage);
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