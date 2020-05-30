package il.co.ilrd.raspi_clients.weather_station;

import java.io.IOException;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.nio.ByteBuffer;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonObject;
import com.google.gson.stream.JsonReader;


public class RecieveMessage implements Runnable {

	private final static String COMMAND_TYPE = "Succsess";
	GsonBuilder builder = new GsonBuilder();
	Gson gson = builder.create();
	JsonReader reader;

	@Override
	public void run() {
		ByteBuffer buffer = ByteBuffer.allocate(WeatherStationIOT.BUFFER_SIZE);

		while(WeatherStationIOT.isRunning) {
			try {
				byte[] receivedbuf = new byte[2048];
				InetAddress address = InetAddress.getByName(WeatherStationIOT.IP);
				System.out.println("receiving");
				DatagramPacket packet = new DatagramPacket(receivedbuf, receivedbuf.length, address, WeatherStationIOT.PORT);
		        WeatherStationIOT.socket.receive(packet);
		        String responseMessage = new String(receivedbuf); 
				String responseKey = getGey(responseMessage);
					if (responseKey != null) {
						WeatherStationIOT.map.remove(responseKey);						
					}
			} catch (IOException e1) {
				System.err.println("Connection Closed!");
				WeatherStationIOT.close();
			}
		}
	}

	private String getGey(String buffer) throws UnsupportedEncodingException {
		reader = new JsonReader(new StringReader(buffer));
		reader.setLenient(true);
		JsonObject j = gson.fromJson(reader, JsonObject.class);
		String[] splittedResponse = j.get(COMMAND_TYPE).getAsString().split("\\$");
		
		return splittedResponse[1].replace("'", "");			
	}
}
