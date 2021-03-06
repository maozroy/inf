package generic_company;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.function.Function;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import il.co.ilrd.gatewayserver.CMDFactory;
import il.co.ilrd.gatewayserver.DatabaseManagementInterface;
import il.co.ilrd.gatewayserver.FactoryCommand;


public class CompanyRegisteration implements il.co.ilrd.gatewayserver.FactoryCommandModifier {
	private static final String SQL_COMMAND = "sqlCommand";
	private static final String DB_NAME = "dbName";
	private final String commandName = "COMPANY_REGISTRATION";
	private final int version = 2;

	private GsonBuilder builder = new GsonBuilder();
	private Gson gson = builder.create();
///
	@Override
	public void addToFactory() {
		CMDFactory<FactoryCommand, String, Object> cmdFactory = CMDFactory.getFactory();
		Function<Object, FactoryCommand> companyRegFunc = (Object a)-> new CompanyRegisterationTask();
		cmdFactory.add(commandName, companyRegFunc);
	}
	
	private class CompanyRegisterationTask implements FactoryCommand{

		@Override
		public String run(Object data, DatabaseManagementInterface databaseManagement) {
			@SuppressWarnings("unchecked")
			HashMap<String, String> json = gson.fromJson(data.toString(), HashMap.class);
			try {
				databaseManagement.createRow(json.get(SQL_COMMAND));
			} catch (SQLException e) {
				return getJsonFormat("error", e.getMessage());
			} catch (NullPointerException e) {
				return getJsonFormat("error", "Wrong fields used");
			}

			return getJsonFormat("Succsess", json.get(DB_NAME) + " Company Registered");
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
