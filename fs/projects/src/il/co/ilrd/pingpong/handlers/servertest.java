package il.co.ilrd.pingpong.handlers;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class servertest {
	public static void main(String[] args) throws ClassNotFoundException, IOException, InterruptedException {
		Server server = new Server();
		server.addTcpConnection(60000, InetAddress.getLocalHost());
//		Thread.sleep(10000);
		server.addBroadcastConnection(50000);
		server.addUdpConnection(5000, InetAddress.getLocalHost());
			new Thread(server).start();

}
}
