package il.co.ilrd.raspi_clients.fridge.http;

import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonObject;
import com.google.gson.stream.JsonReader;

import il.co.ilrd.http_message.HttpParser;


public class RecieveMessage implements Runnable {
	private final static String COMMAND_TYPE = "Succsess";
	GsonBuilder builder = new GsonBuilder();
	Gson gson = builder.create();
	JsonReader reader;
	
	@Override
	public void run() {

		while(FridgeIOT.isRunning) {
			byte[] byteArr = new byte[FridgeIOT.BUFFER_SIZE];
			try (
					InputStream inputStream = FridgeIOT.socket.getInputStream();
				){
				inputStream.read(byteArr);
				String responseMessage = new String(byteArr);
				System.out.println("RESPONSE: " + responseMessage);
				HttpParser httpPareser = new HttpParser(responseMessage);
				String key = getGey(ByteBuffer.wrap(httpPareser.getBody().getBody().getBytes()));
				if (key != null) {
					FridgeIOT.map.remove(key);
				}
			} catch (IOException e1) {
				System.err.println("Connection Closed!");
				FridgeIOT.close();
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
