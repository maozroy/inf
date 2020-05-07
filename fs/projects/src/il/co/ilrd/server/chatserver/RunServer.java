package il.co.ilrd.server.chatserver;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;


public class RunServer {

	public static void main(String[] args) throws ClassNotFoundException, IOException, InterruptedException {
//		ChatServer server = new ChatServer();
//		server.addTcpConnection(ProtocolPort.CHAT_PROTOCOL_PORT.getPort());
//		server.run();
		ChatClientEyal eyalClient = new ChatClientEyal("maoz", "172.20.20.9");
		eyalClient.startClient();
	}
}
