package il.co.ilrd.DatabaseManagement;

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
		CloseConnection();
	}
	
	public void deleteTable(String tableName){
		ConnectToServer();
		ConnectToDB();
		try {
			Statement statement = connection.createStatement();
			statement.executeUpdate("DROP TABLE " + tableName);
		} catch (SQLException e) {
			System.err.println("table " + tableName + " does not exist");
			return;
		}finally {
			CloseConnection();
		}
	}
	
	public void createRow(String sqlCommand) throws SQLException {
		ConnectToServer();
		ConnectToDB();
		Statement statement = connection.createStatement();
		statement.executeUpdate(sqlCommand);
		CloseConnection();
	}
	
	public void createIOTEvent(String rawData) {
		String[] items = rawData.split("\\|");
		try {
			createRow(
					"INSERT INTO " + IOTEVENT_TABLE + " (fk_serial_number, description) " + 
					"VALUES ('" + items[0] + "', '"+ items[1] + "');"
					);
		} catch (SQLException e) {
			System.err.println("IOT data inserted is wrong");
			CloseConnection();
		}
	}
	
	public List<Object> readRow(String tableName, String primaryKeyColumnName, Object primaryKey) throws SQLException {	
		ConnectToServer();
		ConnectToDB();

		ArrayList<Object> list = new ArrayList<Object>();
		ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
		PopulateList(list, resultSet);
		CloseConnection();
		return list;
	}

	public Object readField(String tableName, String primaryKeyColumnName, Object primaryKey, int columnIndex) throws SQLException {
		ConnectToServer();
		ConnectToDB();
		
		ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
		Object retVal = resultSet.getObject(columnIndex);
		CloseConnection();
		return retVal;
	}
	
	public Object readField(String tableName, String primaryKeyColumnName, Object primaryKey, String columnName) throws SQLException {
		ConnectToServer();
		ConnectToDB();
		
		ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
		Object retVal = resultSet.getObject(columnName);
		CloseConnection();
		return retVal;
	}
	
	public void updateField(String tableName, String primaryKeyColumnName, Object primaryKey, int columnIndex, Object newValue) throws SQLException {
		ConnectToServer();
		ConnectToDB();
		ResultSet resultSet = GetResultSet(tableName, primaryKeyColumnName, primaryKey);
		resultSet.updateObject(columnIndex, newValue);
		resultSet.updateRow();
		CloseConnection();
	}
	
	public void updateField(String tableName, String primaryKeyColumnName, Object primaryKey, String columnName, Object newValue) throws SQLException {
		ConnectToServer();
		ConnectToDB();
		Statement statement = connection.createStatement();
		statement.executeUpdate("UPDATE " + tableName + " SET " + primaryEqual(columnName, newValue.toString()) + 
				" WHERE " + primaryEqual(primaryKeyColumnName, primaryKey.toString()));
		CloseConnection();
	}
	
	public void deleteRow(String tableName, String primaryKeyColumnName, Object primaryKey) throws SQLException {
		ConnectToServer();
		ConnectToDB();
		Statement statement = connection.createStatement();
		statement.executeUpdate(BuildCommand("DELETE FROM", tableName, primaryKeyColumnName, primaryKey.toString()));
		CloseConnection();
	}
	
	private String primaryEqual(String columnName, String key) {
		return columnName + " = " + "'" + key + "'";
	}
	
	private void CreateDB() {
		try {
			Statement statement = connection.createStatement();
			statement.executeUpdate("CERATE DATABASE " + databaseName);			
			
		} catch (SQLException e) {}
		finally {
			CloseConnection();
		}
	}
	private void CloseConnection() {
		try {
			connection.close();
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
	
	private void ConnectToDB()  {
		try {
			Statement statement = connection.createStatement();
			statement.executeUpdate("USE " + databaseName);
		} catch (SQLException e) {
			System.err.println("DB does not exist");
		}	
	}
	
	
	private ResultSet GetResultSet(String tableName, String primaryKeyColumnName, Object primaryKeyValue) throws SQLException {
		Statement statement = connection.createStatement(ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_UPDATABLE);
		ResultSet resultSet = statement.executeQuery(
				BuildCommand("SELECT * FROM ", tableName, primaryKeyColumnName, primaryKeyValue.toString()));
		resultSet.next();
		return resultSet;
	}
	
	private String BuildCommand(String command, String tableName, String primaryKeyColumnName, String primaryKey) {
		return command + " " + tableName + " WHERE " + primaryEqual(primaryKeyColumnName, primaryKey);
	}
	
	private void PopulateList(ArrayList<Object> list, ResultSet resultSet) throws SQLException {
		for (int i = 1; i <= resultSet.getMetaData().getColumnCount(); i++) {
			list.add(resultSet.getObject(i));
		}		
	}
}
