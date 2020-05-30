package il.co.ilrd.databasemanagementserver;

public class ServerTest {
	public static void main(String[] args) {
		IOTServer server = new IOTServer();
		server.addTcpConnection(ProtocolPort.DATABASE_MANAGEMENT_PORT.getPort());
		server.run();
	}
}
