package il.co.ilrd.raspi_clients.weather_station;

import il.co.ilrd.raspi_clients.IOTUpdateMessage;


public class HumiditySensor implements Runnable {
	static int msgCounter = 0;
	private static final String SENSOR_NAME = "Humidity";

	@Override
	public void run() {
		System.out.println("in humid");
		while(WeatherStationIOT.isRunning) {
			try {
				Thread.sleep(1000);
				msgCounter++;
				if (msgCounter%5 == 0) {
					SendMessage.addToQueue(new IOTUpdateMessage("ERROR " + msgCounter + "@" + SENSOR_NAME, "Humidity level is too damn high@0", 3, 1));
				}else {
					SendMessage.addToQueue(new IOTUpdateMessage("UPDATE " + msgCounter + "@" + SENSOR_NAME, "Humidity level normal", 1, 1));
				}	
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}		
	}
}
