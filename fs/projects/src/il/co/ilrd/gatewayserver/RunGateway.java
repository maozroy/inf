package il.co.ilrd.gatewayserver;

public class RunGateway {

	public static void main(String[] args) {
		GatewayServer server = new GatewayServer();
		server.addTcpServer(ServerPort.TCP_SERVER_PORT);
		server.addHighHttpServer(ServerPort.HTTP_SERVER_PORT);

		server.start();
		
		
	}

}
