package il.co.ilrd.sunhttpserver;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.URI;
import java.nio.charset.Charset;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;
import com.sun.net.httpserver.Headers;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import il.co.ilrd.http_message.HttpMethod;
import il.co.ilrd.http_message.HttpStatusCode;

public class SunHttpServer {
	private final static String SERVER_URL = "jdbc:mysql://127.0.0.1/";
	private final static String DB_USER = "root";
	private final static String DB_PASS = "root";
	
	private final static String BODY_FIELD_PKNAME ="primaryKeyName";
	private final static String BODY_FIELD_RAW_DATA ="rawData";
	private final static String BODY_FIELD_PKVALUE ="primaryKeyValue";
	private final static String BODY_FIELD_SQLCOMMAND = "sqlCommand";
	private final static String BODY_FIELD_NEW_VALUE = "newValue";
	private final static String BODY_FIELD_COLUMN_NAME ="columName";
	private final static String BODY_FIELD_COLUMN_INDEX ="columnIndex";
	
	private final static String URL_FIELD_PKNAME ="pkName";
	private final static String URL_FIELD_PKVALUE ="pkValue";
	private final static String URL_FIELD_COLUMN_NAME ="columnName";
	private final static String URL_FIELD_COLUMN_INDEX ="columnIndex";
	
	private final static String RESPONSE_NOT_IMPLEMENTED = "Function not Implemented";
	private final static String RESPONSE_BAD_JSON = "Bad Json Format";
	private final static String RESPONSE_BAD_PARAMS = "Bad Params Format";

	private final static String RESPONSE_ERRORMSG = "errorMessage";
	private final static String RESPONSE_MESSAGE = "message";
	private final static String RESPONSE_FIELD_VALUE = "fieldValue";
	private final static String RESPONSE_ROW_VALUES = "rowValues";
	private final static String HANDLER_STRING_ROW = "/row";
	private final static String HANDLER_STRING_TABLE = "/table";
	private final static String HANDLER_STRING_FIELD_NAME = "/field_name";
	private final static String HANDLER_STRING_FIELD_INDEX = "/field_index";
	private final static String HANDLER_STRING_IOT_EVENT = "/iotEvent";
	
	private final static String RESPONSE_HEADER_TYPE = "application/json";
	private final static String RESPONSE_HEADER_ALLOW_OPTIONS = "Allow";
	private final static String RESPONSE_HEADER_SEPERATOR_OPTIONS = ", ";

	
	private HttpServer httpServer;
	private HashMap<String, DatabaseManagement> dbMap = new HashMap<>();
	private HashMap<String, String> bodyMap = new HashMap<>();

	private GsonBuilder builder = new GsonBuilder();
	private Gson gson = builder.create();
	
	protected SunHttpServer(int portNumber) throws IOException {
		httpServer = HttpServer.create(new InetSocketAddress(portNumber), 0);
		httpServer.createContext(HANDLER_STRING_ROW, new RowHandler());
		httpServer.createContext(HANDLER_STRING_TABLE, new TableHandler());
		httpServer.createContext(HANDLER_STRING_FIELD_NAME, new FieldNameHandler());
		httpServer.createContext(HANDLER_STRING_FIELD_INDEX, new FieldIndexHandler());
		httpServer.createContext(HANDLER_STRING_IOT_EVENT, new iotEventHandler());
		httpServer.createContext("/*", new WrongHandler());
	}
	
	protected void start() {
		httpServer.start();
	}
	
	protected void stop() {
		httpServer.stop(1);
	}

	private void handleAllRequests(HashMap<String, FunctionHandler<ParseURL>> funcMap, HttpExchange message) {
		try {
			ParseURL url = new ParseURL(message.getRequestURI());
			AddDataBaseIfNeeded(url.getDatabaseName());
			
			if (funcMap.containsKey(message.getRequestMethod())) {
				funcMap.get(message.getRequestMethod()).run(message, url);
			}else {
				sendResponse(buildBody(RESPONSE_ERRORMSG, RESPONSE_NOT_IMPLEMENTED), message, HttpStatusCode.NOT_IMPLEMENTED);						
			}
		} catch (SQLException e) {
			e.printStackTrace();
			sendResponse(buildBody(RESPONSE_ERRORMSG, e.getMessage()), 
					message, HttpStatusCode.BAD_REQUEST);					
		}catch (IndexOutOfBoundsException | ClassCastException e) {
			e.printStackTrace();
			sendResponse(buildBody(RESPONSE_ERRORMSG, RESPONSE_BAD_PARAMS), 
					message, HttpStatusCode.BAD_REQUEST);					
			} catch (JsonSyntaxException e) {
				e.printStackTrace();
			sendResponse(buildBody(RESPONSE_ERRORMSG, RESPONSE_BAD_JSON), 
					message, HttpStatusCode.BAD_REQUEST);						
		}
	}
	
	//CONTEX HANDLERS//
	
	private class WrongHandler implements HttpHandler{

		@Override
		public void handle(HttpExchange message) throws IOException {
			sendResponse(buildBody(RESPONSE_ERRORMSG, "Bad Handler"), 
					message, HttpStatusCode.NOT_IMPLEMENTED);			
		}
	}
	
	private class FieldNameHandler implements HttpHandler{
		HashMap<String, FunctionHandler<ParseURL>> funcMap = new HashMap<>();
	
		public FieldNameHandler() {
			funcMap.put(HttpMethod.GET.getMethodAsString(), new FieldNameGet());
			funcMap.put(HttpMethod.PUT.getMethodAsString(), new FieldNamePut());
			funcMap.put(HttpMethod.OPTIONS.getMethodAsString(), new FieldNameOptions());
		}

		@Override
		public void handle(HttpExchange message) {
			handleAllRequests(funcMap, message);
		}

		private class FieldNameOptions implements FunctionHandler<ParseURL>{
			
			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException {
				message.getResponseHeaders().add(RESPONSE_HEADER_ALLOW_OPTIONS, 
						HttpMethod.GET.getMethodAsString() + 
						RESPONSE_HEADER_SEPERATOR_OPTIONS + 
						HttpMethod.PUT.getMethodAsString());
				sendResponse(message, HttpStatusCode.OK);						
			}
		}

		private class FieldNameGet implements FunctionHandler<ParseURL>{
			
			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException, JsonSyntaxException {
				VerifyParams(url.getParamMap(), URL_FIELD_PKNAME, URL_FIELD_PKVALUE, URL_FIELD_COLUMN_NAME);
				Object returnVal = dbMap.get(url.getDatabaseName()).readField(
						url.getDatabaseName(), 
						url.getParamMap().get(URL_FIELD_PKNAME), 
						url.getParamMap().get(URL_FIELD_PKVALUE), 
						url.getParamMap().get(URL_FIELD_COLUMN_NAME));
						
				sendResponse(buildBody(returnVal.toString(), RESPONSE_FIELD_VALUE), message, HttpStatusCode.OK);						
			}
		}
		
		private class FieldNamePut implements FunctionHandler<ParseURL>{
			private final static String FIELD_UPDATED = "field updated";

			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException, JsonSyntaxException {
				HashMap<String, String> body = getBodyMap(message);
				dbMap.get(url.getDatabaseName()).updateField(
						url.TableName, 
						body.get(BODY_FIELD_PKNAME), 
						body.get(BODY_FIELD_PKVALUE), 
						body.get(BODY_FIELD_COLUMN_NAME), 
						body.get(BODY_FIELD_NEW_VALUE));
				sendResponse(buildBody(RESPONSE_MESSAGE, FIELD_UPDATED), message, HttpStatusCode.OK);						
			}
		}
	}
	
	private class FieldIndexHandler implements HttpHandler{
		HashMap<String, FunctionHandler<ParseURL>> funcMap = new HashMap<>();
	
		public FieldIndexHandler() {
			funcMap.put(HttpMethod.GET.getMethodAsString(), new FieldIndexGet());
			funcMap.put(HttpMethod.PUT.getMethodAsString(), new FieldIndexPut());
			funcMap.put(HttpMethod.OPTIONS.getMethodAsString(), new FieldIndexOptions());
		}

		@Override
		public void handle(HttpExchange message) {
			handleAllRequests(funcMap, message);
		}

		private class FieldIndexOptions implements FunctionHandler<ParseURL>{
			
			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException {
				message.getResponseHeaders().add(RESPONSE_HEADER_ALLOW_OPTIONS, 
						HttpMethod.GET.getMethodAsString() + 
						RESPONSE_HEADER_SEPERATOR_OPTIONS + 
						HttpMethod.PUT.getMethodAsString());
				sendResponse(message, HttpStatusCode.OK);						
			}
		}

		private class FieldIndexGet implements FunctionHandler<ParseURL>{
			
			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException, JsonSyntaxException {
				VerifyParams(url.getParamMap(), URL_FIELD_PKNAME, URL_FIELD_PKVALUE, URL_FIELD_COLUMN_INDEX);
				Object returnVal = dbMap.get(url.getDatabaseName()).readField(
						url.getDatabaseName(), 
						url.getParamMap().get(URL_FIELD_PKNAME), 
						url.getParamMap().get(URL_FIELD_PKVALUE), 
						toInt(url.getParamMap().get(URL_FIELD_COLUMN_INDEX)));
				sendResponse(buildBody(returnVal.toString(), RESPONSE_FIELD_VALUE), message, HttpStatusCode.OK);						
			}
		}
		
		private class FieldIndexPut implements FunctionHandler<ParseURL>{
			private final static String FIELD_UPDATED = "field updated";

			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException, JsonSyntaxException {
				HashMap<String, String> body = getBodyMap(message);
				dbMap.get(url.getDatabaseName()).updateField(
						url.TableName, 
						body.get(BODY_FIELD_PKNAME), 
						body.get(BODY_FIELD_PKVALUE), 
						toInt(body.get(BODY_FIELD_COLUMN_INDEX)), 
						body.get(BODY_FIELD_NEW_VALUE));
				sendResponse(buildBody(RESPONSE_MESSAGE, FIELD_UPDATED), message, HttpStatusCode.OK);						
			}
		}
	}

	private class TableHandler implements HttpHandler{
		HashMap<String, FunctionHandler<ParseURL>> funcMap = new HashMap<>();
	
		public TableHandler() {
			funcMap.put(HttpMethod.POST.getMethodAsString(), new TablePost());
			funcMap.put(HttpMethod.DELETE.getMethodAsString(), new TableDelete());
			funcMap.put(HttpMethod.OPTIONS.getMethodAsString(), new TableOptions());
		}

		@Override
		public void handle(HttpExchange message) {
			handleAllRequests(funcMap, message);
		}

		private class TableOptions implements FunctionHandler<ParseURL>{
			
			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException {
				message.getResponseHeaders().add(RESPONSE_HEADER_ALLOW_OPTIONS, 
						HttpMethod.DELETE.getMethodAsString() + 
						RESPONSE_HEADER_SEPERATOR_OPTIONS + 
						HttpMethod.POST.getMethodAsString());
				sendResponse(message, HttpStatusCode.OK);						
			}
		}

		private class TableDelete implements FunctionHandler<ParseURL>{
			private final static String TABLE_DELETED = "table deleted";

			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException {
				dbMap.get(url.getDatabaseName()).deleteTable(url.getTableName());
				sendResponse(buildBody(RESPONSE_MESSAGE, TABLE_DELETED), message, HttpStatusCode.OK);						
			}
		}
		
		private class TablePost implements FunctionHandler<ParseURL>{
			private final static String TABLE_CREATED = "table created";

			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException, JsonSyntaxException {
				HashMap<String, String> body = getBodyMap(message);
				dbMap.get(url.getDatabaseName()).createTable(body.get(BODY_FIELD_SQLCOMMAND));
				sendResponse(buildBody(RESPONSE_MESSAGE, TABLE_CREATED), message, HttpStatusCode.OK);						
			}
		}
	}	
	
	private class iotEventHandler implements HttpHandler{
		HashMap<String, FunctionHandler<ParseURL>> funcMap = new HashMap<>();
		
		public iotEventHandler() {
			funcMap.put(HttpMethod.POST.getMethodAsString(), new iotEventPost());
			funcMap.put(HttpMethod.OPTIONS.getMethodAsString(), new iotEventOptions());
		}

		@Override
		public void handle(HttpExchange message) {
			handleAllRequests(funcMap, message);
		}
		
		private class iotEventPost implements FunctionHandler<ParseURL>{
			private final static String IOT_UPDATED = "iot event added";

			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException, JsonSyntaxException {
				HashMap<String, String> body= getBodyMap(message);
				dbMap.get(url.getDatabaseName()).createIOTEvent(body.get(BODY_FIELD_RAW_DATA));
				sendResponse(buildBody(RESPONSE_MESSAGE, IOT_UPDATED), message, HttpStatusCode.OK);						
			}
		}
		
		private class iotEventOptions implements FunctionHandler<ParseURL>{
			
			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException {
				message.getResponseHeaders().add(RESPONSE_HEADER_ALLOW_OPTIONS, 
						HttpMethod.POST.getMethodAsString());
				sendResponse(message, HttpStatusCode.OK);						
			}
		}
	}
	
	private class RowHandler implements HttpHandler{
		HashMap<String, FunctionHandler<ParseURL>> funcMap = new HashMap<>();
	
		public RowHandler() {
			funcMap.put(HttpMethod.GET.getMethodAsString(), new RowGet());
			funcMap.put(HttpMethod.POST.getMethodAsString(), new RowPost());
			funcMap.put(HttpMethod.DELETE.getMethodAsString(), new RowDelete());
			funcMap.put(HttpMethod.OPTIONS.getMethodAsString(), new RowOptions());
		}

		@Override
		public void handle(HttpExchange message) {
			handleAllRequests(funcMap, message);
		}

		private class RowOptions implements FunctionHandler<ParseURL>{
			
			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException {
				message.getResponseHeaders().add(RESPONSE_HEADER_ALLOW_OPTIONS, 
						HttpMethod.GET.getMethodAsString() + 
						RESPONSE_HEADER_SEPERATOR_OPTIONS + 
						HttpMethod.DELETE.getMethodAsString() + 
						HttpMethod.POST.getMethodAsString());
				sendResponse(message, HttpStatusCode.OK);						
			}
		}

		private class RowGet implements FunctionHandler<ParseURL>{
			
			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException {
				VerifyParams(url.getParamMap(), URL_FIELD_PKNAME, URL_FIELD_PKVALUE);
				
				List<Object> returnVal = dbMap.get(url.getDatabaseName()).readRow(
						url.getTableName(),
						url.getParamMap().get(URL_FIELD_PKNAME),
						url.getParamMap().get(URL_FIELD_PKVALUE)
						);
				sendResponse(buildBody(returnVal, RESPONSE_ROW_VALUES), message, HttpStatusCode.OK);	
						
			}
		}
		
		private class RowDelete implements FunctionHandler<ParseURL>{
			private final static String ROW_DELETED = "row deleted";

			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException, JsonSyntaxException {
				HashMap<String, String> body = getBodyMap(message);
				dbMap.get(url.getDatabaseName()).deleteRow(
						url.TableName, 
						body.get(BODY_FIELD_PKNAME), 
						body.get(BODY_FIELD_PKVALUE)
						);
				sendResponse(buildBody(RESPONSE_MESSAGE, ROW_DELETED), message, HttpStatusCode.OK);						
			}
		}
		
		private class RowPost implements FunctionHandler<ParseURL>{
			private final static String ROW_CREATED = "row created";

			@Override
			public void run(HttpExchange message, ParseURL url) throws SQLException, JsonSyntaxException {
				HashMap<String, String> body = getBodyMap(message);
				dbMap.get(url.getDatabaseName()).createRow(body.get(BODY_FIELD_SQLCOMMAND));
				sendResponse(buildBody(RESPONSE_MESSAGE, ROW_CREATED), message, HttpStatusCode.OK);		
			}
		}
		

	}
		
	// SENDING METHODS  //
	
	private String buildBody(List<Object> value, String key) {
		bodyMap.clear();
		bodyMap.put(key, gson.toJson(value, value.getClass()));
		
		return gson.toJson(bodyMap, bodyMap.getClass()).replace("\\", "");
	}
	
	private String buildBody(String key, String value) {
		bodyMap.clear();
		bodyMap.put(key, value);
		String resultJson = gson.toJson(bodyMap, bodyMap.getClass()).replace("\\", "");
		return resultJson;
	}
	
	private void sendResponse(HttpExchange message, HttpStatusCode status) {
		addContentTypeHeader(message.getResponseHeaders());
		
			try {
				message.sendResponseHeaders(status.getCode(), 0);
			} catch (IOException e) {
				System.err.println("Sending response failed");
				e.printStackTrace();
			}
	}
	
	private void sendResponse(String responseBody, HttpExchange message, HttpStatusCode status) {
		addContentTypeHeader(message.getResponseHeaders());
		
		try {
				message.sendResponseHeaders(status.getCode(), responseBody.length());
				OutputStream outStream = message.getResponseBody();
				outStream.write(responseBody.getBytes());
				outStream.close();
		} catch (IOException e) {
			System.err.println("Sending response failed");
			e.printStackTrace();
		}
	}
	
	private void addContentTypeHeader(Headers headers) {
		headers.add("Content-Type", RESPONSE_HEADER_TYPE);
	}
	
	// PRIVATE METHODS //
	
	private void VerifyParams(HashMap<String, String> paramMap, String field1, String field2) {
		if (!(paramMap.containsKey(field2) && paramMap.containsKey(field2))) {
			throw new IndexOutOfBoundsException("Missing Params");
		}
	}
	private void VerifyParams(HashMap<String, String> paramMap, String field1, String field2, String field3) {
		if (!(paramMap.containsKey(field2) && paramMap.containsKey(field2) && paramMap.containsKey(field3))) {
			throw new IndexOutOfBoundsException("Missing Params");
		}
	}
	
	private int toInt(Object obj) {
		return Integer.parseInt(obj.toString());
	}
	
	@SuppressWarnings("unchecked")
	private HashMap<String, String> getBodyMap(HttpExchange message) throws JsonSyntaxException {
		return gson.fromJson(streamToMap(message.getRequestBody()), HashMap.class);
	}
	
	private String streamToMap(InputStream stream) {
		try (Scanner scanner = new Scanner(stream, Charset.forName("UTF-8"))) {
			return scanner.useDelimiter("\\A").next();
		}
	}	

	private void AddDataBaseIfNeeded(String companyName) throws SQLException {
		if (!dbMap.containsKey(companyName)) {
			DatabaseManagement newDb = new DatabaseManagement(SERVER_URL, DB_USER, DB_PASS, companyName);
			dbMap.put(companyName, newDb);		
		}
	}
	
	//URL PARSER//
	
	private class ParseURL {
		private final static String URL_PARAM_DELIMITER = "\\?";
		private final static String URL_DELIMITER = "\\/";
		private final static String PARAM_DELIMITER = "\\&";
		private final static String KEY_VALUE_DELIMITER = "=";

		private String DatabaseName;
		private String TableName;
		private HashMap<String, String> paramMap = new HashMap<>();
		
		public ParseURL(URI url) throws ArrayIndexOutOfBoundsException{
			String[] dbAndTable = url.toString().split(URL_PARAM_DELIMITER)[0].split(URL_DELIMITER);
			String args = url.getRawQuery();
			
			if (args != null) {
				String[] params = args.split(PARAM_DELIMITER);
				String[] paramParts;
				for (int i = 0; i < params.length; i++) {
					paramParts = params[i].split(KEY_VALUE_DELIMITER);
					paramMap.put(paramParts[0], paramParts[1]);
				}
			}
			DatabaseName = dbAndTable[2];
			TableName = dbAndTable[3];
		}
		
		public String getDatabaseName() {
			return DatabaseName;
		}

		public String getTableName() {
			return TableName;
		}

		public HashMap<String, String> getParamMap() {
			return paramMap;
		}
	}
	
	//DB CONNECTOR CLASS//
	
	private class DatabaseManagement {
		private final static String IOTEVENT_TABLE = "IOTEvent";
		private java.sql.Connection connection = null;
		private final String databaseName;
		private final String url;
		private final String userName;
		private final String password;
		
		public DatabaseManagement(String url, String userName, String password, String databaseName)throws SQLException {
			this.databaseName = databaseName;
			this.url = url;
			this.userName = userName;
			this.password = password;
			ConnectToServer();
			CreateDB();
		}
		
		public void createTable(String sqlCommand) throws SQLException {
			ConnectToServer();
			ConnectToDB();
			Statement statement = connection.createStatement();
			statement.executeUpdate(sqlCommand);
			CloseConnection(statement);
		}
		
		public void deleteTable(String tableName) throws SQLException{
			ConnectToServer();
			ConnectToDB();
				Statement statement = connection.createStatement();
				statement.executeUpdate("DROP TABLE " + tableName);
				CloseConnection(statement);
		}
		
		public void createRow(String sqlCommand) throws SQLException {
			ConnectToServer();
			ConnectToDB();
			Statement statement = connection.createStatement();
			statement.executeUpdate(sqlCommand);
			CloseConnection(statement);
		}
		
		public void createIOTEvent(String rawData) throws SQLException {
			String[] items = rawData.split("\\|");
				createRow(
						"INSERT INTO " + IOTEVENT_TABLE + " (fk_serial_number, description) " + 
						"VALUES (" + items[0] + ", "+ items[1] + items[2] + ");"
						);
		}
		
		public List<Object> readRow(String tableName, String primaryKeyColumnName, Object primaryKey) throws SQLException {	
			ConnectToServer();
			ConnectToDB();
	
			ArrayList<Object> list = new ArrayList<Object>();
			ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
			PopulateList(list, resultSet);
			CloseConnection(resultSet);
			return list;
		}
	
		public Object readField(String tableName, String primaryKeyColumnName, Object primaryKey, int columnIndex) throws SQLException {
			ConnectToServer();
			ConnectToDB();
			
			ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
			Object retVal = resultSet.getObject(columnIndex);
			CloseConnection(resultSet);
			return retVal;
		}
		
		public Object readField(String tableName, String primaryKeyColumnName, Object primaryKey, String columnName) throws SQLException {
			ConnectToServer();
			ConnectToDB();
			
			ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
			Object retVal = resultSet.getObject(columnName);
			CloseConnection(resultSet);
			return retVal;
		}
		
		public void updateField(String tableName, String primaryKeyColumnName, Object primaryKey, int columnIndex, Object newValue) throws SQLException {
			ConnectToServer();
			ConnectToDB();
			ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
			resultSet.updateObject(columnIndex, newValue);
			resultSet.updateRow();
			CloseConnection(resultSet);
		}
		
		public void updateField(String tableName, String primaryKeyColumnName, Object primaryKey, String columnName, Object newValue) throws SQLException {
			ConnectToServer();
			ConnectToDB();
			ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
			resultSet.updateObject(columnName, newValue);
			resultSet.updateRow();
			CloseConnection(resultSet);
		}
		
		public void deleteRow(String tableName, String primaryKeyColumnName, Object primaryKey) throws SQLException {
			ConnectToServer();
			ConnectToDB();
			Statement statement = connection.createStatement();
			statement.executeUpdate(BuildQueryCommand("DELETE FROM", tableName, primaryKeyColumnName, primaryKey.toString()));
			CloseConnection(statement);
		}
		
	
		
		private void CreateDB() {
				try {
					Statement statement = connection.createStatement();
					statement.executeUpdate("CREATE DATABASE IF NOT EXISTS  " + databaseName);
					CloseConnection(statement);
				} catch (SQLException e) {}		
		}
		
		private void CloseConnection(ResultSet resultSet) {
			try {
				connection.close();
				resultSet.close();
			} catch (SQLException e) {
				System.err.println("closing connection failed");
			}
		}
		
		private void CloseConnection(Statement statement) {
			try {
				connection.close();
				statement.close();
			} catch (SQLException e) {
				System.err.println("closing connection failed");
			}
		}
		
		private void ConnectToServer() {
			try {
				connection = DriverManager.getConnection(url, userName, password);
			} catch (SQLException e) {
				System.err.println("Server connection failed");
			}
		}
		
		private void ConnectToDB() throws SQLException  {
				Statement statement = connection.createStatement();
				statement.executeUpdate("USE " + databaseName);
		}
		
		
		private ResultSet GetResultSet(String tableName, String primaryKeyColumnName, Object primaryKeyValue) throws SQLException {
			Statement statement = connection.createStatement(ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_UPDATABLE);
			ResultSet resultSet = statement.executeQuery(
					BuildQueryCommand("SELECT * FROM ", tableName, primaryKeyColumnName, primaryKeyValue.toString()));
			resultSet.next();
			return resultSet;
		}
		
		private String BuildQueryCommand(String command, String tableName, String primaryKeyColumnName, String primaryKey) {
			return command + " " + tableName + " WHERE " + primaryEqual(primaryKeyColumnName, primaryKey);
		}
		
		private String primaryEqual(String columnName, String key) {
			return columnName + " = " + key;
		}
		
		private void PopulateList(ArrayList<Object> list, ResultSet resultSet) throws SQLException {
			for (int i = 1; i <= resultSet.getMetaData().getColumnCount(); i++) {
				list.add(resultSet.getObject(i));
		}		
	}
}
	
}
interface FunctionHandler<T>{
	public void run (HttpExchange message, T url) throws SQLException, JsonSyntaxException;
}