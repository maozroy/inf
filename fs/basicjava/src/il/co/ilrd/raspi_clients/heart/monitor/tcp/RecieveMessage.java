package il.co.ilrd.raspi_clients.heart.monitor.tcp;

import java.io.IOException;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
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
		ByteBuffer buffer = ByteBuffer.allocate(HeartMonitorIOT.BUFFER_SIZE);

		while(HeartMonitorIOT.isRunning) {
			try {
				buffer.clear();
				if (HeartMonitorIOT.clientSocket.read(buffer) != -1) {
					buffer.flip();
					String responseKey = getGey(buffer);
					if (responseKey != null) {
						HeartMonitorIOT.map.remove(responseKey);						
					}
				}
			} catch (IOException e1) {
				System.err.println("Connection Closed!");
				HeartMonitorIOT.close();
			}
		}
	}

	private String getGey(ByteBuffer buffer) throws UnsupportedEncodingException {
		reader = new JsonReader(new StringReader(new String(buffer.array(), "UTF-8")));
		reader.setLenient(true);
		JsonObject j = gson.fromJson(reader, JsonObject.class);
		String[] splittedResponse = j.get(COMMAND_TYPE).getAsString().split("\\$");
		return splittedResponse[1].replace("'", "");			
	}
}
