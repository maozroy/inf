package il.co.ilrd.gatewayserver;

import java.io.IOException;

public class RunGateway {

	public static void main(String[] args) throws InterruptedException, IOException {
		GatewayServer server = new GatewayServer(5, "/home/student/jar/new");
		server.addTcpServer(ServerPort.TCP_SERVER_PORT);
		server.addHighHttpServer(ServerPort.HTTP_SERVER_PORT);
//	//	server.addLowHttpServer(ServerPort.UDP_SERVER_PORT);
//		server.addUdpServer(ServerPort.UDP_SERVER_PORT);
		new Thread(server).start();
		Thread.sleep(4000);
//		server.setNumOfThreads(10);
//		server.setNumOfThreads(2);
//		server.setNumOfThreads(0);
		System.out.println("stopped");
		server.stop();
//		System.err.println("ERR");
	}

}
