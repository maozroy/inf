package il.co.ilrd.raspi_clients.heart.monitor.tcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;

public class RecieveMessage implements Runnable {
	Selector selector;

	@Override
	public void run() {
		SocketChannel client = null;
		ByteBuffer buffer = ByteBuffer.allocate(1000);
		
		
		while(HeartMonitorIOT.isRunning) {
			try {
				client = SocketChannel.open(new InetSocketAddress(InetAddress.getByName(HeartMonitorIOT.IP), HeartMonitorIOT.PORT));
				buffer.clear();
				client.read(buffer);
				String response = new String(buffer.array(), "UTF-8");
				String key = getGey(response);
				HeartMonitorIOT.map.remove(key);

				System.out.println(response);
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
	}
}
