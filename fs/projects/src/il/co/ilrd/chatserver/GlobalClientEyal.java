package il.co.ilrd.chatserver;

import java.io.*;

import il.co.ilrd.server.general.ProtocolPort;

public class GlobalClientEyal {
	public static void main(String[] args) throws ClassNotFoundException, IOException, InterruptedException {
		new GlobalChatClient("maoz", "172.20.20.10").startClient();
//		ChatServer server = new ChatServer();
//		server.addTcpConnection(ProtocolPort.CHAT_PROTOCOL_PORT.getPort());
//		server.run();
	}

}