package il.co.ilrd.databasemanagementserver;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.InvalidObjectException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.channels.Channel;
import java.nio.channels.ClosedSelectorException;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;


public class IOTServer implements Runnable{

	private ByteBuffer buffer;
	private boolean started = false;
	private boolean killed = false;
	private ConnectionHandler connectHandler = new ConnectionHandler();
	private MessageHandler msgHandler = new MessageHandler();
	
	@Override
	public void run() {
		if (!started) {			
			started = true;
			new Thread(new ExitServer()).start();
			try {
				connectHandler.startConnections();
			} catch (IOException e) {
				e.getCause();
			}
		}
	}
	
	public void stopServer() {
		try {
			connectHandler.stopConnections();
		} catch (IOException e) {
			System.err.println("Closing connections failed");
		}
	}

	public void addBroadcastConnection(int portNumber) throws UnknownHostException{
		checkIfStarted();
		connectHandler.addConnection(new BroadcastConnection(portNumber,connectHandler.selector));
	}


	public void addTcpConnection(int portNumber){
		checkIfStarted();
		connectHandler.addConnection(new TcpConnection(portNumber));
	}

	public void addUdpConnection(int portNumber, InetAddress address){
		checkIfStarted();
		connectHandler.addConnection(new UdpConnection(portNumber));
	}
		
	private void checkIfStarted() {
		if (started) {
			throw new ExceptionInInitializerError("Server Already started");
		}
	}
	
	private class ExitServer implements Runnable{
		private static final String KILL_STRING = "exit";

		@Override
		public void run() {
			try(BufferedReader buffer = new BufferedReader
					(new InputStreamReader(System.in))) {
				
				while(!buffer.readLine().equals(KILL_STRING)) {}
				killed = true;
				connectHandler.stopConnections();
			} catch (IOException e) {e.printStackTrace();} 						
		}
		
	}
	

	/**********************************************
	 * Connection Handler
	 **********************************************/
	private class ConnectionHandler {
		private Selector selector;
		private LinkedList<Connection> connections = new LinkedList<>();
		private HashMap<Channel, Connection> channelmap = new HashMap<>();
		private static final int BUFFER_SIZE = 32768;
		private static final long TIMEOUT_PRINT = 20000;
		
		private void startConnections() throws IOException  {
			buffer = ByteBuffer.allocate(BUFFER_SIZE);

			try {
				selector = Selector.open();
			} catch (IOException e) {
				System.err.println("Selector open failed");
			}
			
			for (Connection connection : connections) {
				connection.initConnection(selector);
			}

			try {
				while (!killed) {
					while (0 == selector.select(TIMEOUT_PRINT)) {
						System.out.println("Server is waiting");
					}
					Set<SelectionKey> selectedKeys = selector.selectedKeys();
					Iterator<SelectionKey> iter = selectedKeys.iterator();
					
					while (iter.hasNext()) {
						SelectionKey key = iter.next();
						if (key.isValid() && key.channel().isOpen()) {
							Connection connection = channelmap.get(key.channel());
							if (key.isAcceptable() ) {
								registerTcpClient(key.channel(),connection);
							}
							if (key.isReadable()) {
								connection.bufferToMsgHandler(key, connection);
							}
						}
						iter.remove();
					}
				}
			} catch (ClosedSelectorException e) {
				if (killed) {					
					System.out.println("Server Exiting");
					return;
				}
			}
		}

		private void stopConnections() 
				throws IOException {
			
			Iterator<SelectionKey> key = selector.keys().iterator();
			while(key.hasNext()) {
				key.next().channel().close();
			}	
			selector.close();
		}

		private void addConnection(Connection connection) {
			connections.add(connection);
		}
		
		private void registerTcpClient(Channel channel, Connection connection) {
			SocketChannel tcpClient; 
			
			try {
				tcpClient = ((ServerSocketChannel) channel).accept();
				tcpClient.configureBlocking(false);
				tcpClient.register(selector, SelectionKey.OP_READ);
				channelmap.put(tcpClient, connection);
				((TcpConnection)connection).socketInfo.put(tcpClient, new ClientInfo(tcpClient, connection));
			} catch (IOException e) {
				System.err.println("TCP Register failed");
				e.printStackTrace();
			}
		}
	}
	
	/**********************************************
	 * Connection Interface
	 **********************************************/

	interface Connection {
		public void sendMessage(ByteBuffer message, ClientInfo info) throws IOException;
		public void bufferToMsgHandler(SelectionKey key, Connection connection) 
				throws IOException;
		public void initConnection(Selector selector); 
		public int getPort();
		public Closeable getSocket();

	}

	/**********************************************
	 * TCP Connection
	 **********************************************/
	private class TcpConnection implements Connection {
		private int port;
		private ServerSocketChannel tcpSocket;
		private Map<SocketChannel, ClientInfo> socketInfo = new HashMap<>();

		
		public TcpConnection(int port)  {
			this.port = port;
		}

		@Override
		public void sendMessage(ByteBuffer message, ClientInfo info) throws IOException {
			
			info.gettcpSocket().write(message);
			message.clear();
			
		}

		@Override
		public void bufferToMsgHandler(SelectionKey key, Connection connection) {
			TcpConnection tcpConnection = (TcpConnection) connection;
			
			try {
				SocketChannel client = (SocketChannel) key.channel();

				if (-1 == client.read(buffer)) {
					client.close();
				}else {
					msgHandler.handleMessage(buffer, tcpConnection.socketInfo.get(client));
				}
			} catch (IOException e) {
				System.err.println("Connection closed by client");
			}finally {
				buffer.clear();
			}
		}

		@Override
		public int getPort() {
			return port;
		}
		
		@Override
		public String toString() {
			return "TcpConnection [" + tcpSocket + "]" ;
		}

		@Override
		public Closeable getSocket() {
			return tcpSocket;
		}

		@Override
		public void initConnection(Selector selector) {
			try {
				tcpSocket = ServerSocketChannel.open();
				tcpSocket.configureBlocking(false);
				tcpSocket.bind(new InetSocketAddress(port));
				tcpSocket.register(selector, SelectionKey.OP_ACCEPT);	
				connectHandler.channelmap.put(tcpSocket, this);
			} catch (IOException e) {
				System.err.println("TCP on port "+ port + "failed");
				e.printStackTrace();
			}			
		}

	}
	/**********************************************
	 * UDP Connection
	 **********************************************/
	private class UdpConnection implements Connection {
		private DatagramChannel udpSocket;
		private int port;
		
		public UdpConnection(int port) {
			this.port = port;
		}

		@Override
		public void initConnection(Selector selector) {
			try {
				udpSocket = DatagramChannel.open();
				udpSocket.configureBlocking(false);
				udpSocket.socket().bind(new InetSocketAddress(port));
				udpSocket.register(selector, SelectionKey.OP_READ);
				connectHandler.channelmap.put(udpSocket, this);
			} catch (IOException e) {
				System.err.println("UDP on port "+ port + "failed");
			}	
		}
		
		@Override
		public void sendMessage(ByteBuffer message, ClientInfo info) {
			try {
				udpSocket.send(message, info.getudpSocket());
			} catch (IOException e) {
				System.err.println("UDP to " + info + "Failed");
			}
		}

		@Override
		public void bufferToMsgHandler(SelectionKey key, Connection connection) {
				SocketAddress clientAddress;
				try {
					clientAddress = udpSocket.receive(buffer);
					msgHandler.handleMessage(buffer, new ClientInfo(clientAddress, connection));
				} catch (IOException e) {
					System.err.println("receiving UDP message failed");
				}
				buffer.clear();
		}

		@Override
		public int getPort() {
			return port;
		}
		
		@Override
		public String toString() {
			return "UdpConnection [" + udpSocket + "]" + "in port" + port ;
		}

		@Override
		public Closeable getSocket() {
			return udpSocket;
		}

	}
	
	/**********************************************
	 * Broadcast Connection
	 **********************************************/
	private class BroadcastConnection extends UdpConnection {

		public BroadcastConnection(int port, Selector selector) throws UnknownHostException {
			super(port);
		}
	}
	
	/**********************************************
	 * Protocol
	 **********************************************/
	interface Protocol {
		public void handleMessage(ClientInfo info, Message<?, ?> msg);
	}

	/**********************************************
	 * DB Protocol
	 **********************************************/
private class DBProtocol implements Protocol{
	private final static String SERVER_URL = "jdbc:mysql://localhost/";
	private final static String DB_USER = "root";
	private final static String DB_PASS = "root";
	
	private static final String WRONG_PARAMS_USED_STRING = "Params not suitable for key";
	private static final String NULL_COMPANY_NAME = "Company name cannot be null";
	private static final String WRONG_PORT_USED = "Used wrong port";

	private final ServerMessage ack_msg = new ServerMessage(ProtocolType.DATABASE_MANAGEMENT, new DatabaseManagementMessage(new ActionTypeKey(null, DatabaseKeys.ACK_MESSAGE), null));
	private final ServerMessage err_msg = new ServerMessage(ProtocolType.DATABASE_MANAGEMENT, new DatabaseManagementMessage(new ActionTypeKey(null, DatabaseKeys.ERROR_MESSAGE), null));
	private LinkedList<Object> wrapperList = new LinkedList<Object>();
	private HashMap<DatabaseKeys, DBFunction> functionMap = new HashMap<>();
	private HashMap<String, DatabaseManagement> companyMap = new HashMap<>();
	private List<Object> recievedParams;
	
	public DBProtocol() {
		initFunctionMap();
	}

	@Override
	public void handleMessage(ClientInfo info, Message<?, ?> msg) {
		String dbName = null;
		try {
			dbName = GetCompanyName((DatabaseManagementMessage) msg.getData());
			if (info.connection.getPort() != ProtocolPort.DATABASE_MANAGEMENT_PORT.getPort()) {
				setAndSendMessage(err_msg, info, WRONG_PORT_USED, dbName);
			}else {
				
				DatabaseManagementMessage dbMsg = (DatabaseManagementMessage) msg.getData();
				functionMap.get(dbMsg.getKey().getActionType()).apply(info, dbMsg, dbName);			
			}
		} catch (NoComapnyName e) {
			setAndSendMessage(err_msg, info, NULL_COMPANY_NAME, null);	
		} catch (SQLException e) {
			setAndSendMessage(err_msg, info, e.getMessage(), dbName);
		} catch (ClassCastException | IndexOutOfBoundsException e) {
			setAndSendMessage(err_msg, info, WRONG_PARAMS_USED_STRING, dbName);
		}
	}

	// -- HANDLERS --
	
	private class CreateDatabase implements DBFunction{
		private static final String ACK_STRING_CREATED = "DataBase created";
		private static final String ACK_STRING_ALREADY_EXISTS = "DataBase already exists";

		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			if (!companyMap.containsKey(companyName)) {
				AddDataBaseNoCheck(companyName);
				setAndSendMessage(ack_msg, info ,ACK_STRING_CREATED, companyName);
			} else {
				setAndSendMessage(ack_msg, info ,ACK_STRING_ALREADY_EXISTS, companyName);
			}	
		}
	}
	
	private class CreateTable implements DBFunction{
		private final static String CREATE_SUCCESS = "Table created";
		
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			AddDataBase(companyName);
			String sqlCommand = getIndexString(message.getData(), 0);
			companyMap.get(companyName).createTable(sqlCommand);
			setAndSendMessage(ack_msg, info, CREATE_SUCCESS, companyName);
		}
	}
	
	private class CreateRow implements DBFunction{
		private final static String CREATE_ROW_SUCCESS = "Row Added";

		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			AddDataBase(companyName);
			companyMap.get(companyName).createRow(getIndexString(message.getData(), 0));
			setAndSendMessage(ack_msg, info, CREATE_ROW_SUCCESS, companyName);
		}
	}
	
	private class CreateIOTEvent implements DBFunction{
		private final static String IOT_EVENT_SUCCESS = "IOT event added";

		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
				AddDataBase(companyName);
				companyMap.get(companyName).createIOTEvent(getIndexString(message.getData(), 0));
				setAndSendMessage(ack_msg, info, IOT_EVENT_SUCCESS, companyName);
		}
	}
	
	
	private class UpdateByName implements DBFunction{
		private final static String UPDATE_ACK = "Update sucsses";
		
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			recievedParams = message.getData();
			AddDataBase(companyName);
			companyMap.get(companyName).updateField(
					getIndexString(recievedParams, 0), 
					getIndexString(recievedParams, 1), 
					getIndexObject(recievedParams, 2), 
					getIndexString(recievedParams, 3),
					getIndexObject(recievedParams, 4));
			setAndSendMessage(ack_msg, info, UPDATE_ACK, companyName);
		}	
	}
	
	private class UpdateByIndex implements DBFunction{
		private final static String UPDATE_ACK = "Update sucsses";
		
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			recievedParams = message.getData();
			AddDataBase(companyName);
			companyMap.get(companyName).updateField(
					getIndexString(recievedParams, 0), 
					getIndexString(recievedParams, 1), 
					getIndexObject(recievedParams, 2), 
					getIndexInt(recievedParams, 3),
					getIndexObject(recievedParams, 4));
			setAndSendMessage(ack_msg, info, UPDATE_ACK, companyName);	
		}	
	}
	
	private class ReadRow implements DBFunction{
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			recievedParams = message.getData();
			AddDataBase(companyName);
			List<Object> returnVal = companyMap.get(companyName).readRow(
					getIndexString(recievedParams, 0), 
					getIndexString(recievedParams, 1), 
					getIndexObject(recievedParams, 2));
			setAndSendMessage(ack_msg, info, returnVal, companyName);
		}
	}

	private class ReadFieldByIndex implements DBFunction{
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			recievedParams = message.getData();
			AddDataBase(companyName);
			Object returnVal = companyMap.get(companyName).readField(
					getIndexString(recievedParams, 0), 
					getIndexString(recievedParams, 1), 
					getIndexObject(recievedParams, 2), 
					getIndexInt(recievedParams, 3));
			setAndSendMessage(ack_msg, info, returnVal, companyName);
		}
	}
		
	private class ReadFieldByName implements DBFunction{
		
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName)
				throws SQLException {	
			recievedParams = message.getData();
			AddDataBase(companyName);
			Object returnVal = companyMap.get(companyName).readField(
					getIndexString(recievedParams, 0), 
					getIndexString(recievedParams, 1), 
					getIndexObject(recievedParams, 2), 
					getIndexString(recievedParams, 3));
			setAndSendMessage(ack_msg, info, returnVal, companyName);	
		}
	}
	
	private class DeleteRow implements DBFunction{
		private final static String DELETE_ROW_ACK = "Deleted!";
		
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			recievedParams = message.getData();
			AddDataBase(companyName);
			companyMap.get(companyName).deleteRow(
					getIndexString(recievedParams, 0), 
					getIndexString(recievedParams, 1), 
					getIndexObject(recievedParams, 2));
			setAndSendMessage(ack_msg, info, DELETE_ROW_ACK, companyName);
		}
	}
	
	private class DeleteTable implements DBFunction{
		private final static String DELETE_SUCCESS = "Table deleted";
		
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) 
				throws SQLException {	
			AddDataBase(companyName);
			String tableName = getIndexString(message.getData(),0);
			companyMap.get(companyName).deleteTable(tableName);
			setAndSendMessage(ack_msg, info, DELETE_SUCCESS, companyName);
		}
	}	
	
	private class WrongKey implements DBFunction{
		private final static String WRONG_KEY = "Wrong key used";
		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) {	
			setAndSendMessage(err_msg, info, WRONG_KEY, companyName);	
		}
	}
	
	// -- HELPER METHODS --
	
	private void initFunctionMap() {
		WrongKey wrongKey = new WrongKey();
		functionMap.put(DatabaseKeys.CREATE_COMPANY_DATABASE, new CreateDatabase());
		functionMap.put(DatabaseKeys.CREATE_TABLE, new CreateTable());
		functionMap.put(DatabaseKeys.DELETE_TABLE, new DeleteTable());
		functionMap.put(DatabaseKeys.CREATE_IOT_EVENT, new CreateIOTEvent());
		functionMap.put(DatabaseKeys.CREATE_ROW, new CreateRow());
		functionMap.put(DatabaseKeys.READ_ROW, new ReadRow());
		functionMap.put(DatabaseKeys.READ_FIELD_BY_NAME, new ReadFieldByName());
		functionMap.put(DatabaseKeys.READ_FIELD_BY_INDEX, new ReadFieldByIndex());
		functionMap.put(DatabaseKeys.UPDATE_FIELD_BY_NAME, new UpdateByName());
		functionMap.put(DatabaseKeys.UPDATE_FIELD_BY_INDEX, new UpdateByIndex());
		functionMap.put(DatabaseKeys.DELETE_ROW, new DeleteRow());
		functionMap.put(DatabaseKeys.ERROR_MESSAGE, wrongKey);
		functionMap.put(DatabaseKeys.ACK_MESSAGE, wrongKey);		
	}
	
	
	private void SendMessage(ClientInfo info, ServerMessage msg) {
		try {
			buffer.clear();
			buffer.put(ConversionUtils.toByteArray(msg));
			buffer.flip();
			info.connection.sendMessage(buffer, info);
		} catch (IOException e) {
			System.err.println("sending to client: " + info + "Failed");
		}
	}
	
	private String getIndexString(List<Object> data, int index) throws ClassCastException{
		return (String)data.get(index);
	}
	
	private Object getIndexObject(List<Object> data, int index) throws ClassCastException{
		return data.get(index);
	}
	
	private int getIndexInt(List<Object> data, int index) throws ClassCastException{
		return (int)data.get(index);
	}
	
	private void AddDataBase(String companyName) throws SQLException {
		if (!companyMap.containsKey(companyName)) {
			DatabaseManagement newDb = new DatabaseManagement(SERVER_URL, DB_USER, DB_PASS, companyName);
			companyMap.put(companyName, newDb);		
		}
	}
	
	private void AddDataBaseNoCheck(String companyName) throws SQLException {
		DatabaseManagement newDb = new DatabaseManagement(SERVER_URL, DB_USER, DB_PASS, companyName);
		companyMap.put(companyName, newDb);		
	}

	private String GetCompanyName(DatabaseManagementMessage message) throws NoComapnyName{
		String name = message.getKey().getDatabaseName();
		if (null == name) {
			throw new NoComapnyName();
		}
		return name;
	}

	private void setAndSendMessage(ServerMessage msg, ClientInfo info, Object data, String companyMame) {
		wrapperList.add(data);
		setAndSendMessage(msg,info, wrapperList, companyMame);
		wrapperList.clear();
	}
	
	private void setAndSendMessage(ServerMessage msg, ClientInfo info, List<Object> data, String companyMame) {
		((DatabaseManagementMessage)msg.getData()).getKey().setDatabaseName(companyMame);
		((DatabaseManagementMessage)msg.getData()).setData(data);
		SendMessage(info, msg);
	}
	
	private class NoComapnyName extends Exception{
		private static final long serialVersionUID = -3362339935031338949L;

		public NoComapnyName() {
			super();
		}
	}
	
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
	
private interface DBFunction{
	public void apply(ClientInfo info, DatabaseManagementMessage message, String companyName) throws SQLException;
}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private HashMap<ProtocolType, Protocol> protocolMap = new HashMap<ProtocolType, Protocol>();;
		
		public MessageHandler() {
			protocolMap.put(ProtocolType.DATABASE_MANAGEMENT, new DBProtocol());
		}

		void handleMessage(ByteBuffer message, ClientInfo info) {

			ServerMessage msg;
			try {
				msg = (ServerMessage) ConversionUtils.toObject(message.array());
				System.out.println("msg recieved: "+msg);
				protocolMap.get(msg.getKey()).handleMessage(info, (Message<?, ?>) msg);
			} catch (ClassNotFoundException e) {
				System.err.println("Protocol Not Found");
				return;
			} catch (IOException e) {
				System.err.println("Message Decryption failed");
				e.printStackTrace();

			}
		}
	}
	/***********************************************
	 * Client info
	 **********************************************/
	private class ClientInfo{
		private SocketChannel tcpSocket;
		private SocketAddress udpSocket;
		private Connection connection;
		
		public SocketChannel gettcpSocket() {
			return tcpSocket;
		}

		public SocketAddress getudpSocket() {
			return udpSocket;
		}

		public ClientInfo(SocketAddress udpSocket, Connection connection) {
			this.udpSocket = udpSocket;
			this.connection = connection;
		}
		
		public ClientInfo(SocketChannel tcpSocket, Connection connection) {
			this.tcpSocket = tcpSocket;
			this.connection = connection;
		}

		@Override
		public String toString() {
			return "ClientInfo [tcpSocket = " + tcpSocket + ", udpSocket = " + udpSocket + ", connection = " + connection + "]";
		}
	}
}


