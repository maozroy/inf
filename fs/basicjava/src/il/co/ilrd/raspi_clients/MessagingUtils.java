package il.co.ilrd.raspi_clients;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

import org.json.JSONException;
import org.json.JSONObject;


public class MessagingUtils {
	public static final String DB_NAME_FIELD = "dbName";
	public static final String KEY_COMMAND_FIELD = "CommandKey";
	public static final String IOT_UPDATE = "IOT_UPDATE";
	public static final String RAW_DATA = "rawData";
	public static final String APOSTROPHE = "'";
	public static final String TIME_DATE_PATTERN = "yyyy-MM-dd HH:mm:ss";
	public static final String DELIMETER = "|";
	public static final String DATA = "Data";
	
	
	public static String prepareMessage(String currentMessage, String dbName, int serialNumber) {
		JSONObject innerJsonObject = new JSONObject();
		try {
			innerJsonObject.put(DB_NAME_FIELD, dbName);
			innerJsonObject.put(RAW_DATA,
					APOSTROPHE + 
					serialNumber + 
					APOSTROPHE + 
					DELIMETER + 
					APOSTROPHE + 
					currentMessage + 
					APOSTROPHE +  
					DELIMETER +
					APOSTROPHE + 
					getCurrentTime() + 
					APOSTROPHE );
			JSONObject jsonObject = new JSONObject();
			jsonObject.put(
					KEY_COMMAND_FIELD, 
					IOT_UPDATE);
			jsonObject.put(DATA, innerJsonObject);
			return jsonObject.toString();
		} catch (JSONException e) {
			System.err.println("Message format from server is wrong!");
			e.printStackTrace();
		}
		return null;
	}
	
	private static String getCurrentTime() {
		DateTimeFormatter timeFormatter = DateTimeFormatter.ofPattern(TIME_DATE_PATTERN);
		return timeFormatter.format(LocalDateTime.now()); 
	}
}
