package il.co.ilrd.pingpong.handlers;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class servertest {
	public static void main(String[] args) throws ClassNotFoundException, IOException, InterruptedException {
		SharonServer server = new SharonServer();
	//	server.addUdpConnection(5000, InetAddress.getLocalHost());
		server.addTcpConnection(60000);
		server.addBroadcastConnection(5000);
//		Thread.sleep(10000);
			server.run();

}
}
