package il.co.ilrd.raspi_clients.weather_station;

import java.io.IOException;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.InetAddress;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonObject;
import com.google.gson.stream.JsonReader;

import il.co.ilrd.raspi_clients.IOTUpdateMessage;
import il.co.ilrd.raspi_clients.MessagingUtils;
import il.co.ilrd.raspi_clients.heart.monitor.tcp.HeartMonitorIOT;


public class RecieveMessage implements Runnable {

	private final static String COMMAND_TYPE = "Succsess";
	GsonBuilder builder = new GsonBuilder();
	Gson gson = builder.create();
	JsonReader reader;

	@Override
	public void run() {

		while(WeatherStationIOT.isRunning) {
			try {
				byte[] receivedbuf = new byte[2048];
				InetAddress address = InetAddress.getByName(WeatherStationIOT.IP);
				DatagramPacket packet = new DatagramPacket(receivedbuf, receivedbuf.length, address, WeatherStationIOT.PORT);
		        WeatherStationIOT.socket.receive(packet);
		        String responseMessage = new String(receivedbuf); 
				String id = getMessageID(responseMessage);
				MessagingUtils.TurnLedLong();
					if (id != null) {
						WeatherStationIOT.semQueueMsgs.tryAcquire();
						IOTUpdateMessage currentMsg = WeatherStationIOT.idToIOTMap.get(id);
						WeatherStationIOT.RTT = MessagingUtils.calculateRTT(WeatherStationIOT.RTT, System.currentTimeMillis() - currentMsg.getTimeSent());
						WeatherStationIOT.queue.remove(currentMsg);
						WeatherStationIOT.idToIOTMap.remove(id);
					}
			} catch (IOException e1) {
				System.err.println("Connection Closed!");
				WeatherStationIOT.close();
			}
		}
	}

	private String getMessageID(String buffer) throws UnsupportedEncodingException {
		reader = new JsonReader(new StringReader(buffer));
		reader.setLenient(true);
		JsonObject j = gson.fromJson(reader, JsonObject.class);
		String[] splittedResponse = j.get(COMMAND_TYPE).getAsString().split("\\$");
		
		return splittedResponse[1].replace("'", "");			
	}
}
