package il.co.ilrd.raspi_clients.fridge.http;

import il.co.ilrd.raspi_clients.IOTUpdateMessage;

public class DoorSensor implements Runnable {
	int msgCounter = 0;
	private final static String SENSOR_NAME = "doorSensor";
	
	
	public void run() {
		while(FridgeIOT.isRunning) {
			try {
				Thread.sleep(5000);
				msgCounter++;
				if (msgCounter%5 == 0) {
					SendMessage.addToQueue(new IOTUpdateMessage("ERROR " + msgCounter + "@" + SENSOR_NAME, "door is open!", 3, 1));
				}else {
					SendMessage.addToQueue(new IOTUpdateMessage("UPDATE " + msgCounter + "@" + SENSOR_NAME, "door closed", 1, 1));
				}			
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}		
	}

}
