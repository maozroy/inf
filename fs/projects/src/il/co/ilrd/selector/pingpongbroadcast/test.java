package il.co.ilrd.selector.pingpongbroadcast;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;


public class test {

	public static void main(String[] args) throws Exception {
		TcpUdpBroadcastPongServer server = new TcpUdpBroadcastPongServer();
		server.RunServer();
	}

}
