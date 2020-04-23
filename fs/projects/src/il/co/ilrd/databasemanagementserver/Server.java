package il.co.ilrd.databasemanagementserver;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
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

import il.co.ilrd.server.general.*;

public class Server implements Runnable{

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
	private HashMap<DatabaseKeys, DBFunction> functionMap = new HashMap<>();
	private HashMap<String, DatabaseManagement> companyMap = new HashMap<>();
	
	
	CREATE_COMPANY_DATABASE,
	CREATE_TABLE,
	DELETE_TABLE,
	CREATE_IOT_EVENT,
	CREATE_ROW,
	READ_ROW,
	READ_FIELD_BY_NAME,
	READ_FIELD_BY_INDEX,
	UPDATE_FIELD_BY_NAME,
	UPDATE_FIELD_BY_INDEX,
	DELETE_ROW,
	ERROR_MESSAGE,
	ACK;
	public DBProtocol() {
//		WrongHandler wrongHandler = new WrongHandler();
		functionMap.put(DatabaseKeys.CREATE_COMPANY_DATABASE, new CreateDatabase());
		functionMap.put(DatabaseKeys.CREATE_TABLE, new CreateTable());
		functionMap.put(DatabaseKeys.DELETE_TABLE, new DeleteTable());
		functionMap.put(DatabaseKeys.CREATE_IOT_EVENT, new CreateIOTEvent());
		functionMap.put(DatabaseKeys.CREATE_ROW, new CreateRow());
		functionMap.put(DatabaseKeys.READ_FIELD_BY_NAME, new ReadFieldByName());
		functionMap.put(DatabaseKeys.READ_FIELD_BY_INDEX, new ReadFieldByIndex());
		functionMap.put(DatabaseKeys.UPDATE_FIELD_BY_NAME, new UpdateByName());
		functionMap.put(DatabaseKeys.UPDATE_FIELD_BY_INDEX, new UpdateByIndex());
		functionMap.put(DatabaseKeys.DELETE_ROW, new DeleteRow());
		functionMap.put(DatabaseKeys.UPDATE_FIELD_BY_NAME, new UpdateByName());
		functionMap.put(DatabaseKeys.ERROR_MESSAGE, WrongKey);
		functionMap.put(DatabaseKeys.ACK, WrongKey);
		}
	
	@Override
	public void handleMessage(ClientInfo info, Message<?, ?> msg) {
		if (info.connection.getPort() != ProtocolPort.DATABASE_MANAGEMENT_PORT.getPort()) {
			//TODO send error msg
		return;
		}
	
		functionMap.get(((DatabaseManagementMessage)msg.getData()).getKey()).apply(info,(DatabaseManagementMessage)msg.getData());
	}
	
	private class CreateDatabase implements DBFunction{

		@Override
		public void apply(ClientInfo info, DatabaseManagementMessage message) {
			// TODO Auto-generated method stub
			
		}
		
	}
	
	private class DatabaseManagement {
		private final static String IOTEVENT_TABLE = "IOTEvent";
		private java.sql.Connection connection = null;
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
}
	
	private interface DBFunction{
		public void apply(ClientInfo info, DatabaseManagementMessage message);
	}

	//		private HashMap<ChatProtocolKeys, ChatFunction> funcionMap = new HashMap<>();
//		private LinkedList<ChatClientInfo> userList = new LinkedList<>();
//		private ServerMessage defaultError = new ServerMessage(ProtocolType.CHAT_SERVER, 
//				new ChatServerMessage(ChatProtocolKeys.ERROR_MESSAGE, "User Does not exist"));
//		
//		private static final String USER_NOT_FOUND = "User Not Found";
//		private static final String WRONG_KEY = "Key used is inaccurate";
//		private final static String REG_FAILED = "Registration Failed";
//		
//		public ChatProtocol() {
//			WrongHandler wrongHandler = new WrongHandler();
//			funcionMap.put(ChatProtocolKeys.REGISTRATION_REQUEST, new RegistrationRequest());
//			funcionMap.put(ChatProtocolKeys.MESSAGE, new SendingMessage());
//			funcionMap.put(ChatProtocolKeys.REMOVE_REQUEST, new RemoveRequest());
//			funcionMap.put(ChatProtocolKeys.REGISTRATION_ACK, wrongHandler);
//			funcionMap.put(ChatProtocolKeys.REGISTRATION_REFUSE, wrongHandler);
//			funcionMap.put(ChatProtocolKeys.NEW_CLIENT_REGISTRATION, wrongHandler);
//			funcionMap.put(ChatProtocolKeys.BROADCAST_MESSAGE, wrongHandler);
//			funcionMap.put(ChatProtocolKeys.ERROR_MESSAGE, wrongHandler);
//		}
//		
//		@Override
//		public void handleMessage(ClientInfo info, Message<?, ?> msg) {
//			
//			if (info.connection.getPort() != ProtocolPort.CHAT_PROTOCOL_PORT.getPort()) {
//				System.err.println("Port/ProtocolKey combination is wrong");
//				SetMessage(defaultError, "Wrong port used to call Chat. please use " + 
//																		ProtocolPort.CHAT_PROTOCOL_PORT.getPort() + "instead" + 
//																		info.connection.getPort());
//				SendMessage(defaultError, info);
//				return;
//			}
//			
//			funcionMap.get(((ChatServerMessage)msg.getData()).getKey()).apply(info,(ChatServerMessage)msg.getData());
//		}
//		
//		private class RegistrationRequest implements ChatFunction{
//			private ServerMessage regAck = new ServerMessage(ProtocolType.CHAT_SERVER, new ChatServerMessage(ChatProtocolKeys.REGISTRATION_ACK, ""));
//			private ServerMessage regRefuse = new ServerMessage(ProtocolType.CHAT_SERVER, new ChatServerMessage(ChatProtocolKeys.REGISTRATION_REFUSE, REG_FAILED));
//			private static final String HAS_JOINED = " Has Joined";
//
//			@Override
//			public void apply(ClientInfo info, ChatServerMessage message) {
//				String requestedUser =  message.getData();
//				
//				try {
//					if (isUserAvailable(info, requestedUser)) {
//						SetMessage(regAck, requestedUser);
//						SendMessage(regAck, info);
//						sendToAllUsers(new String().concat(requestedUser).concat(HAS_JOINED), 
//										ChatProtocolKeys.NEW_CLIENT_REGISTRATION);
//						userList.add(new ChatClientInfo(info, requestedUser));
//					}else {
//						SendMessage(regRefuse, info);
//					}
//				} catch (IOException e) {
//					System.err.println("put to Buffer failed");
//				}
//			}
//		}
//		
//		private class SendingMessage implements ChatFunction{
//			private ServerMessage bcMsg = new ServerMessage(ProtocolType.CHAT_SERVER, new ChatServerMessage(ChatProtocolKeys.BROADCAST_MESSAGE, ""));
//
//			@Override
//			public void apply(ClientInfo info, ChatServerMessage message) {
//				if (!isUserExists(info)) {
//					SetMessage(defaultError, USER_NOT_FOUND);
//					SendMessage(defaultError, info);
//					return;
//				}
//				
//				SetMessage(bcMsg, findChatClient(info).getUserName() + ": " + message.getData());
//				System.out.println("msg to send: " + bcMsg.getData());
//				for (ChatClientInfo chatClientInfo : userList) {
//					if (info != chatClientInfo.clientInfo) {
//						SendMessage(bcMsg, chatClientInfo.getServerClientInfo());
//					}
//				}
//			}
//
//			private boolean isUserExists(ClientInfo info) {
//				for (ChatClientInfo chatClientInfo : userList) {
//					if (chatClientInfo.getServerClientInfo().equals(info)) {
//						return true;
//					}
//				}	
//				return false;
//			}
//		}
//		
//		private class RemoveRequest implements ChatFunction{
//			private static final String HAS_LEFT = " Has Left";
//			
//			@Override
//			public void apply(ClientInfo info, ChatServerMessage message) {
//				
//				ChatClientInfo chatClient = findChatClient(info);
//				
//				try {
//					if (userList.remove(chatClient)) {
//						sendToAllUsers(new String().concat(chatClient.getUserName()).concat(HAS_LEFT), 
//										ChatProtocolKeys.BROADCAST_MESSAGE);
//					}else {
//						SetMessage(defaultError, USER_NOT_FOUND);
//						SendMessage(defaultError, info);
//					}
//				} catch (IOException e) {
//					System.err.println("Sending remove message failed");
//				}
//			}
//		}
//		
//		private class WrongHandler implements ChatFunction{
//			@Override
//			public void apply(ClientInfo info, ChatServerMessage message) {
//				SetMessage(defaultError, WRONG_KEY);
//				SendMessage(defaultError, info);
//			}
//		}
//		
//		private boolean isUserAvailable(ClientInfo requestingClient, String requestedUser) {
//			for (ChatClientInfo existingUser : userList) {
//				if (existingUser.getUserName().equals(requestedUser) || 
//					requestingClient.gettcpSocket().equals(existingUser.getServerClientInfo().gettcpSocket())) {
//					return false;
//				}
//			}
//			return true;
//		}
//		
//		public void SetMessage(ServerMessage dest, ChatServerMessage src) {
//			((ChatServerMessage)dest.getData()).setMessage(src.getData());			
//		}
//
//		private void SetMessage(ServerMessage dest, String src) {
//			((ChatServerMessage)dest.getData()).setMessage(src);
//		}
//
//		private void sendToAllUsers(String string, ChatProtocolKeys key) 
//				throws IOException {
//			ServerMessage notificationMsg = new ServerMessage(ProtocolType.CHAT_SERVER, 
//											new ChatServerMessage(key, string));
//			for (ChatClientInfo clientInfo : userList) {
//				SendMessage(notificationMsg, clientInfo.getServerClientInfo());
//			}
//		}
//		private void SendMessage(ServerMessage notificationMsg, ClientInfo clientInfo) {
//			ChatClientInfo user = findChatClient(clientInfo);
//			if (!clientInfo.gettcpSocket().isConnected()) {
//				System.out.println(user.getUserName() + " left the Chat");
//				userList.remove(user);
//			}
//			try {
//				buffer.clear();
//				buffer.put(ServerMessage.toByteArray(notificationMsg));
//				buffer.flip();
//				clientInfo.connection.sendMessage(buffer, clientInfo);			
//			} catch (IOException e) {
//				System.err.println("Error sending message to " + user.getUserName());
//			}
//		}
//		
//		private ChatClientInfo findChatClient(ClientInfo info) {
//			for (ChatClientInfo chatClientInfo : userList) {
//				if (chatClientInfo.getServerClientInfo().equals(info)){
//					return chatClientInfo;
//				}
//			}
//			return null;
//		}
//		
//		private class ChatClientInfo{
//			private ClientInfo clientInfo;
//			private String userName;
//			
//			public ChatClientInfo(ClientInfo clientInfo, String userName) {
//				this.clientInfo = clientInfo;
//				this.userName = userName;
//			}
//			
//			public ClientInfo getServerClientInfo() {
//				return clientInfo;
//			}
//	
//			public String getUserName() {
//				return userName;
//			}
//		}
//	}
//	
//	private interface ChatFunction{
//		public void apply(ClientInfo info, ChatServerMessage message);
//	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private HashMap<ProtocolType, Protocol> protocolMap = new HashMap<ProtocolType, Protocol>();;
		
		public MessageHandler() {
			protocolMap.put(ProtocolType.CHAT_SERVER, new DBProtocol());
		}

		void handleMessage(ByteBuffer message, ClientInfo info) {

			ServerMessage msg;
			try {
				msg = (ServerMessage) ServerMessage.toObject(message.array());
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


