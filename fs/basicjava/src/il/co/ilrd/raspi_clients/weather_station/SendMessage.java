package il.co.ilrd.raspi_clients.weather_station;

import java.util.Set;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.util.Map.Entry;


public class SendMessage implements Runnable {

	@Override
	public void run() {
		while(WeatherStationIOT.isRunning) {
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Set<Entry<String, String>> set  = WeatherStationIOT.map.entrySet();
			for (Entry<String, String> entry : set) {
				String rawData = entry.getValue() + "$" + entry.getKey();
				sendToServer(rawData);
				if (entry.getKey().contains("UPDATE")) {
					WeatherStationIOT.map.remove(entry.getKey());
				}
			}
		}
	}

	private void sendToServer(String rawData) {
        DatagramPacket packet;
		try {
			packet = new DatagramPacket(rawData.getBytes(), 
					rawData.getBytes().length, InetAddress.getByName(WeatherStationIOT.IP), WeatherStationIOT.PORT);
			WeatherStationIOT.socket.send(packet);		
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
