package il.co.ilrd.raspi_clients.heart.monitor.tcp;

import il.co.ilrd.raspi_clients.IOTUpdateMessage;

public class HeartrateSensor implements Runnable {
	static int msgCounter = 0;
	private static final String SENSOR_NAME = "heartrate";

	@Override
	public void run() {
		while(HeartMonitorIOT.isRunning) {
			try {
				Thread.sleep(5000);
				msgCounter++;
				if (msgCounter%5 == 0) {
					SendMessage.addToQueue(new IOTUpdateMessage("ERROR " + msgCounter + "@" + SENSOR_NAME, "heart rate is 200", 3, 1));
				}else {
					SendMessage.addToQueue(new IOTUpdateMessage("UPDATE " + msgCounter + "@" + SENSOR_NAME, "heart rate is 100", 1, 1));
				}			
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}		
	}
}
