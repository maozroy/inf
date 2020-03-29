package il.co.ilrd.selector.tcpudpserver;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;


public class test {

	public static void main(String[] args) throws Exception {
		TamirSelector server = new TamirSelector();
		server.startServer();
	}

}
