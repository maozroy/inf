package il.co.ilrd.raspi_clients.heart.monitor.tcp;

import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Map.Entry;
import java.util.Set;

public class SendMessage implements Runnable {
	
	@Override
	public void run() {
		
		while(HeartMonitorIOT.isRunning) {
			Set<Entry<String, String>> set  = HeartMonitorIOT.map.entrySet();
			for (Entry<String, String> entry : set) {
				sendToServer(entry.getValue());
				if (entry.getKey().contains("UPDATE")) {
					HeartMonitorIOT.map.remove(entry.getKey());
				}
			}
		}

	}

	private void sendToServer(String value) {
		ByteBuffer buffer = ByteBuffer.allocate(1000);
	}
		try (
				SocketChannel clientSocket = SocketChannel.open(
									new InetSocketAddress(HeartMonitorIOT.IP, HeartMonitorIOT.PORT));	
		)
		{
			buffer.put(message.getBytes());
			buffer.flip();
			clientSocket.write(writeBuffer);
			writeBuffer.clear();
	}

}
