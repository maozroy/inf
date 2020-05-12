package il.co.ilrd.gatewayserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.HashMap;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import il.co.ilrd.databasemanagementserver.ConversionUtils;
import il.co.ilrd.databasemanagementserver.ServerMessage;
import il.co.ilrd.server.chatserver.ChatProtocolKeys;
import il.co.ilrd.server.chatserver.ChatServerMessage;
import il.co.ilrd.server.chatserver.ProtocolPort;
import il.co.ilrd.server.chatserver.ProtocolType;

public class TcpClient {
	boolean isAlive = true;
	public static void main(String[] args) throws InterruptedException, IOException {
		GsonBuilder builder = new GsonBuilder();
		Gson gson = builder.create();
		ByteBuffer buffer = ByteBuffer.allocate(4096);

		
		HashMap<String, String> message = new HashMap<>();
		message.put("Commandkey", "COMPANY_REGISTRATION");
		message.put("Data", "dayumn");

		
		String toSend = gson.toJson(message, message.getClass());


		InetSocketAddress address = new InetSocketAddress(InetAddress.getLocalHost(), ServerPort.TCP_SERVER_PORT.getPort());
		SocketChannel client = null;
		buffer.put(toSend.getBytes());
		buffer.flip();
		try {
			client = SocketChannel.open(address);
			client.write(buffer);
			buffer.clear();
			client.read(buffer);
			String response = new String(buffer.array(), "UTF-8");
			System.out.println(response);
			

		} catch (IOException e1) {
			e1.printStackTrace();
		}  
		
		
//		try {
//			SocketChannel clientSocket = SocketChannel.open(new InetSocketAddress(ServerPort.TCP_SERVER_PORT.getPort()));
//
//			buffer.clear();
//			buffer.put(toSend.getBytes());
//			buffer.flip();
//			clientSocket.write(buffer);
//		} catch (IOException e1) {
//			e1.printStackTrace();
//		}		
		
//		String hostName = InetAddress.getLocalHost().getHostName();
//		int portNumber = ServerPort.TCP_SERVER_PORT.getPort();
//		ByteBuffer buffer = ByteBuffer.allocate(4096);
//		SocketChannel clientSocket = null;
//
//		ServerMessage receivedMssage = null;
//		byte[] messageArray =  toSend.getBytes();
//		
//		try {
//			clientSocket = SocketChannel.open(new InetSocketAddress(hostName, portNumber));
//		} catch (IOException e1) {
//			e1.printStackTrace();
//		}
//		// send register message
////		messageArray =  ConversionUtils.toByteArray(HTTPMsg);
//		buffer.clear();
//		buffer.put(messageArray);
//		buffer.flip();
//		clientSocket.write(buffer);
//		
		
		
//		try(Socket socket = new Socket(InetAddress.getLocalHost(), ServerPort.TCP_SERVER_PORT.getPort());
//			PrintWriter output = new PrintWriter(socket.getOutputStream());
//			BufferedReader input = new BufferedReader(
//									new InputStreamReader(
//									socket.getInputStream()))
//			){		
//			for (int i = 0 ; i < 5 ;i++) {
//				buffer.clear();
//				buffer.put(messageArray);
//				buffer.flip();
//				clientSocket.write(buffer);
//				output.write(toSend);
//				output.flush();
//				System.out.println("server sends: " + input.readLine());
//			}
//		socket.close();	
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		} 
		
	}
}
