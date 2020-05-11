package il.co.ilrd.sunhttpserver;

import java.io.IOException;

public class ServerTest {
	public static void main(String[] args) throws IOException {
		SunHttpServer server = new SunHttpServer(8080);
		server.start();
		
	}
}
