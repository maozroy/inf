package il.co.ilrd.raspi_clients.heart.monitor.tcp;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Map.Entry;

import il.co.ilrd.raspi_clients.MessagingUtils;

import java.util.Set;

public class SendMessage implements Runnable {
	ByteBuffer buffer = ByteBuffer.allocate(HeartMonitorIOT.BUFFER_SIZE);

	@Override
	public void run() {
		while(HeartMonitorIOT.isRunning) {
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Set<Entry<String, String>> set  = HeartMonitorIOT.map.entrySet();
			for (Entry<String, String> entry : set) {
				String rawData = entry.getValue() + "$" + entry.getKey();
				sendToServer(rawData);
				if (entry.getKey().contains("UPDATE")) {
					HeartMonitorIOT.map.remove(entry.getKey());
				}
			}
		}
	}

	private void sendToServer(String value) {
		try {
			HeartMonitorIOT.clientSocket.write(
			ByteBuffer.wrap((MessagingUtils.prepareMessage(value, HeartMonitorIOT.DB_NAME, HeartMonitorIOT.serialNumber).getBytes())));
		} catch (IOException e) {
			HeartMonitorIOT.close();
		}	
	}
}
