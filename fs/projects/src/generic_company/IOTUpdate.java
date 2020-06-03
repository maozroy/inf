package generic_company;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.function.Function;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import il.co.ilrd.gatewayserver.CMDFactory;
import il.co.ilrd.gatewayserver.DatabaseManagementInterface;
import il.co.ilrd.gatewayserver.FactoryCommand;
import il.co.ilrd.gatewayserver.FactoryCommandModifier;

public class IOTUpdate implements FactoryCommandModifier {
	private static final String RAW_DATA = "rawData";
	private static final String DB_NAME = "dbName";
	private final String commandName = "IOT_UPDATE";
	private final int version = 1;



	private GsonBuilder builder = new GsonBuilder();
	private Gson gson = builder.create();

	@Override
	public void addToFactory() {
		CMDFactory<FactoryCommand, String, Object> cmdFactory = CMDFactory.getFactory();
		Function<Object, FactoryCommand> companyRegFunc = (Object a)-> new IOTUpdateTask();
		cmdFactory.add(commandName, companyRegFunc);
	}
	
	private class IOTUpdateTask implements FactoryCommand{

		@Override
		public String run(Object data, DatabaseManagementInterface databaseManagement) {
			@SuppressWarnings("unchecked")
			HashMap<String, String> json = gson.fromJson(data.toString(), HashMap.class);

			try {
				String rawData = json.get(RAW_DATA);
				String[] splittedData = rawData.split("\\|");
				databaseManagement.createIOTEvent(json.get(RAW_DATA));
				return getJsonFormat("Succsess", splittedData[1]);
			} catch (SQLException e) {
				return getJsonFormat("error", e.getMessage());
			} catch (NullPointerException e) {
				return getJsonFormat("error", "Wrong fields used");
			}
			
		}
		
		private String getJsonFormat(String key, String value) {
			HashMap<String, String> hashMap = new HashMap<String, String>();
			hashMap.put(key, value);
			return gson.toJson(hashMap, hashMap.getClass());
		}
	}
	
	@Override
	public int getVersion() {
		return version;
	}

	@Override
	public String getCommand() {
		return commandName;
	}
}
