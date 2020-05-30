package il.co.ilrd.raspi_clients.fridge.http;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;

import com.google.gson.JsonObject;

import il.co.ilrd.http_message.HttpBuilder;
import il.co.ilrd.http_message.HttpMethod;
import il.co.ilrd.http_message.HttpVersion;
import il.co.ilrd.raspi_clients.MessagingUtils;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;


public class SendMessage implements Runnable {
	private Map<String, String> headersMap = new HashMap<>();

	@Override
	public void run() {
		headersMap.put("Content-Type", "application/json");
		try(
				OutputStream outputStream = FridgeIOT.socket.getOutputStream();)
		{
			while(FridgeIOT.isRunning) {
				Thread.sleep(3000);
				sendMessages(outputStream);
				
			}
		} catch (IOException | InterruptedException e) {
			System.err.println(e.getMessage());
		}	
		
	}

	private void sendMessages(OutputStream outputStream) throws IOException {
		Set<Entry<String, String>> set  = FridgeIOT.map.entrySet();
		for (Entry<String, String> entry : set) {
			String rawData = MessagingUtils.prepareMessage(entry.getValue() + "$" + entry.getKey(), FridgeIOT.DB_NAME, FridgeIOT.serialNumber);
			JsonObject dataobj = new JsonObject();
			dataobj.addProperty(MessagingUtils.RAW_DATA, rawData);
			System.out.println(dataobj.toString());
			headersMap.put("Content-Length",Integer.toString(dataobj.toString().length()));
			
			String httpMessage = HttpBuilder.createHttpRequestMessage(
					HttpMethod.POST, 
					HttpVersion.HTTP_1_1, 
					"/", 
					headersMap, 
					dataobj.toString());
			outputStream.write(httpMessage.getBytes());
			outputStream.flush();
			if (entry.getKey().contains("UPDATE")) {
				FridgeIOT.map.remove(entry.getKey());
			}
		}
		
	}


}
