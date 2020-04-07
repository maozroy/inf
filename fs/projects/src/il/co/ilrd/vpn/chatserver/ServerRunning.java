package il.co.ilrd.vpn.chatserver;

public class ServerRunning {
public static void main(String[] args) {
	ChatServer server = new ChatServer();
	server.addTcpConnection(GlobalProtocolPort.CHAT_PROTOCOL_PORT.getPort());
	server.run();
}
}
