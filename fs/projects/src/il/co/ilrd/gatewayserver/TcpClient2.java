package il.co.ilrd.gatewayserver;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SocketChannel;
import java.util.HashMap;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

public class TcpClient2 {

	public static void main(String[] args) throws IOException {
		
		GsonBuilder builder = new GsonBuilder();
		Gson gson = builder.create();
		ByteBuffer buffer = ByteBuffer.allocate(4096);

		
		HashMap<String, String> message = new HashMap<>();
		message.put("Commandkey", "COMPANY_REGISTRATION");
		message.put("Data", "dayumn");

		
		String toSend = gson.toJson(message, message.getClass());
			
			String inetAddress = "127.0.0.1";
			try (
					SocketChannel clientSocket = SocketChannel.open(
							new InetSocketAddress(inetAddress, ServerPort.TCP_SERVER_PORT.getPort()));	
					)
			{
				buffer.put(toSend.getBytes());
				buffer.flip();
				clientSocket.write(buffer);
				buffer.clear();
				ByteBuffer byteBuffer = ByteBuffer.allocate(4096);//ByteBuffer.wrap(bodyString.getBytes());//Charset.forName("ASCII").encode(bodyString);
				clientSocket.read(byteBuffer);
				String responseString = new String(byteBuffer.array());
				System.out.println(responseString);
			} 
			
			System.out.println("\nnudp");
			for (int i = 0; i < 100000; i++) {
			try (DatagramSocket socket = new DatagramSocket()) {
				byte[] buf = toSend.getBytes();
				InetAddress address = InetAddress.getByName(inetAddress);
				DatagramPacket packet = new DatagramPacket(buf, 
						buf.length, address, 
						ServerPort.UDP_SERVER_PORT.getPort());
				socket.send(packet);
				byte[] receivedbuf = new byte[2048];
				packet = new DatagramPacket(receivedbuf, receivedbuf.length);
				socket.receive(packet);
				System.out.println(new String(receivedbuf));
			}
		} 
		}
}

