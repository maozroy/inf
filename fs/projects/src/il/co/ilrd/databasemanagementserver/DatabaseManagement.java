package il.co.ilrd.databasemanagementserver;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class DatabaseManagement {
	private final static String IOTEVENT_TABLE = "IOTEvent";
	private Connection connection = null;
	private final String databaseName;
	private final String url;
	private final String userName;
	private final String password;
	
	public DatabaseManagement(String databaseName, String url, String userName, String password) throws ClassNotFoundException, SQLException {
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
					"VALUES ('" + items[0] + "', '"+ items[1] + "');"
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
	

	
	private void CreateDB() throws SQLException {
			Statement statement = connection.createStatement();
			statement.executeUpdate("CERATE DATABASE " + databaseName);			
			CloseConnection(statement);
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
		return columnName + " = " + "'" + key + "'";
	}
	
	private void PopulateList(ArrayList<Object> list, ResultSet resultSet) throws SQLException {
		for (int i = 1; i <= resultSet.getMetaData().getColumnCount(); i++) {
			list.add(resultSet.getObject(i));
		}		
	}
}
