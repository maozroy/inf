package il.co.ilrd.pingpong.handlers;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class servertest {
	public static void main(String[] args) throws ClassNotFoundException, IOException, InterruptedException {
		Server server = new Server();
//		server.addTcpConnection(60000);
//		Thread.sleep(10000);
			new Thread(server).start();
			server.addBroadcastConnection(5000);
			server.addUdpConnection(5000, InetAddress.getLocalHost());

}
}
