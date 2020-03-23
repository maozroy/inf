package il.co.ilrd.pingpongtcp;

import java.util.concurrent.Semaphore;

public class tcppingpong {
	static Semaphore firstRun = new Semaphore(0);

	public static void main(String[] args) {
		new Thread(new TcpPongServer()).start();
		new Thread(new TcpPingClient()).start();

	}

}
