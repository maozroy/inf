package il.co.ilrd.server.chatserver;

import java.net.InetAddress;
import java.net.UnknownHostException;

import il.co.ilrd.server.general.ProtocolPort;

public class RunServer {

	public static void main(String[] args) throws UnknownHostException {
		ChatServer server = new ChatServer();
		server.addTcpConnection(ProtocolPort.CHAT_PROTOCOL_PORT.getPort(), InetAddress.getLocalHost());
		server.run();
	}

}
