package il.co.ilrd.server.chatserver;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

import il.co.ilrd.server.general.ProtocolPort;
import il.co.ilrd.server.general.ProtocolType;
import il.co.ilrd.server.general.ServerMessage;

public class ChatClient {

	
	public static void main(String[] args) throws IOException, ClassNotFoundException {
		ByteBuffer buffer = ByteBuffer.allocate(2048); 
		InetSocketAddress address = new InetSocketAddress(InetAddress.getLocalHost(), ProtocolPort.CHAT_PROTOCOL_PORT.getPort());
		SocketChannel client = null;
		ServerMessage regMsg = new ServerMessage(ProtocolType.CHAT_SERVER, new ChatServerMessage(ChatProtocolKeys.REGISTRATION_REQUEST, "maoz"));
		ServerMessage ansMsg;
		buffer.put(ServerMessage.toByteArray(regMsg));
		buffer.flip();
		try {
			client = SocketChannel.open(address);
			client.write(buffer);
			buffer.clear();
			client.read(buffer);
			ansMsg = (ServerMessage) ServerMessage.toObject(buffer.array());
			System.out.println((ChatServerMessage)ansMsg.getData());
			

		} catch (IOException e1) {
			e1.printStackTrace();
		}  
	}
}
