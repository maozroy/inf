package il.co.ilrd.httpiotserver;

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
import java.nio.charset.Charset;
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

import com.google.gson.*;

import il.co.ilrd.databasemanagementserver.DatabaseKeys;
import il.co.ilrd.http_message.HttpBuilder;
import il.co.ilrd.http_message.HttpMethod;
import il.co.ilrd.http_message.HttpParser;
import il.co.ilrd.http_message.HttpStatusCode;
import il.co.ilrd.http_message.HttpVersion;

public class HttpServer implements Runnable{

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
	//s
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

	public void addHttpConnection(int portNumber){
		checkIfStarted();
		connectHandler.addConnection(new HTTPConnection(portNumber));
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
	 * HTTP Connection
	 **********************************************/
	private class HTTPConnection extends TcpConnection {

		public HTTPConnection(int port) {
			super(port);
		}

		@Override
		public void bufferToMsgHandler(SelectionKey key, Connection connection) {
			TcpConnection tcpConnection = (TcpConnection) connection;
			try {
				SocketChannel client = (SocketChannel) key.channel();
				System.out.println("IP SENDING IS: " + ((ServerSocketChannel)connection.getSocket()).getLocalAddress());

				if (-1 == client.read(buffer)) {
					client.close();
				}else {
					buffer = ConvertHttpToSrvrMsg(buffer);
					msgHandler.handleMessage(buffer, tcpConnection.socketInfo.get(client));
				}
			} catch (IOException e) {
				System.err.println("Connection closed by client");
			}finally {
				buffer.clear();
			}		
		}

		private ByteBuffer ConvertHttpToSrvrMsg(ByteBuffer buffer)  {
			try {
				buffer.flip();
				String httpMsg = new String(Charset.forName("ASCII").decode(buffer).array()); //ConversionUtils.toObject(buffer.array());
				ServerMessage msg = new ServerMessage(ProtocolType.DB_HTTP, new HttpMessage(null, httpMsg));
				buffer.clear();
				buffer.put(ConversionUtils.toByteArray(msg));
				buffer.flip();
			} catch (IOException e) {
				e.printStackTrace();
			}
			return buffer;
		}


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
	 * DB_HTTP Protocol
	 **********************************************/
	private class DBHttpProtocol implements Protocol{
		private final static String SERVER_URL = "jdbc:mysql://127.0.0.1/";
		private final static String DB_USER = "root";
		private final static String DB_PASS = "root";
		
		private final static String HEADER_ALLOW = "Allow";
		private final static String HEADER_ALLOWED_METHODS = "GET, POST, PUT, DELETE, OPTION";
		private final static String HEADER_CONNECTION = "Connection";
		private final static String HEADER_CONNECTION_TYPE = "Connection-Type";
		private final static String HEADER_CONTENT_LENGTH = "Content-Length";
		
		private final static String RESPONSE_ERRORMSG = "errorMessage";
		private final static String RESPONSE_MESSAGE = "message";
		private final static String RESPONSE_FIELD_VALUE = "fieldValue";
		private final static String RESPONSE_ROW_VALUES = "rowValues";

		
		private final static String URL_FIELD_PKNAME ="pkName";
		private final static String URL_FIELD_PKVALUE ="pkValue";
		private final static String URL_FIELD_COLUMN_NAME ="columnName";
		private final static String URL_FIELD_COLUMN_INDEX ="columnIndex";
		
		private final static String BODY_FIELD_PKNAME ="primaryKeyName";
		private final static String BODY_FIELD_RAW_DATA ="rawData";
		private final static String BODY_FIELD_PKVALUE ="primaryKeyValue";
		private final static String BODY_FIELD_SQLCOMMAND = "sqlCommand";
		private final static String BODY_FIELD_NEW_VALUE = "newValue";
		private final static String BODY_FIELD_COLUMN_NAME ="columName";
		private final static String BODY_FIELD_COLUMN_INDEX ="columnIndex";

		private GsonBuilder builder = new GsonBuilder();
		private Gson gson = builder.create();
		
		private HashMap<String, String> responseMap = new HashMap<>();
		private HashMap<String, DatabaseManagement> companyMap = new HashMap<>();	
		private HashMap<String, String> responseHeader = new HashMap<>();
		private HashMap<HttpMethod, DBFunction<ParsedHttpMessage>> methodMap = new HashMap<>();
		private DBFunction<ParsedHttpMessage> methodHandle;
		private HttpMessage httpMessage;
		private HttpParser parsedHttp;
		private ParsedHttpMessage parsedMsg;
		
		public DBHttpProtocol() {
			initMethodMap();
			initResponseHeader();
		}

		private void initResponseHeader() {
			responseHeader.put(HEADER_CONNECTION, "close");
			responseHeader.put(HEADER_CONNECTION_TYPE, "application/json");			
		}

		@Override
		public void handleMessage(ClientInfo info, Message<?, ?> msg) {

			
			try {
				httpMessage = (HttpMessage)msg.getData();
				parsedHttp = new HttpParser(httpMessage.getData());
				parsedMsg = new ParsedHttpMessage(parsedHttp);
				
				methodHandle = methodMap.get(parsedMsg.getHttpMethod());
				if (methodHandle != null) {
					methodHandle.apply(info, parsedMsg);
				} else {
					SendResponse(info, HttpStatusCode.BAD_REQUEST, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, "No key Method"));
				}
			} catch (SQLException e) {
				SendResponse(info, HttpStatusCode.BAD_REQUEST, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, e.getMessage()));
			} catch (ArrayIndexOutOfBoundsException | ClassCastException e) {
				SendResponse(info, HttpStatusCode.BAD_REQUEST, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, "wrong paramaters"));
			} catch (JsonParseException e) {
				SendResponse(info, HttpStatusCode.BAD_REQUEST, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, "bad Json format"));
			} catch (com.google.gson.stream.MalformedJsonException e) {
				SendResponse(info, HttpStatusCode.BAD_REQUEST, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, "bad Json format"));
			} catch (NoSuchFieldError e) {
				SendResponse(info, HttpStatusCode.BAD_REQUEST, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, "No Body Sent"));
			}
		}

		private void initMethodMap() {
		DBFunction<ParsedHttpMessage> notImplementedMethod = new NotImplementedMethod();
		methodMap.put(HttpMethod.GET, new GetMethod());
		methodMap.put(HttpMethod.POST, new PostMethod());
		methodMap.put(HttpMethod.PUT, new PutMethod());
		methodMap.put(HttpMethod.DELETE, new DeleteMethod());
		methodMap.put(HttpMethod.OPTIONS, new OptionsMethod());
		methodMap.put(HttpMethod.TRACE, notImplementedMethod);
		methodMap.put(HttpMethod.PATCH, notImplementedMethod);
		methodMap.put(HttpMethod.HEAD, notImplementedMethod);
	}
		
	//-----HTTP METHODS START -----//
		
	private void useFunction(DBFunction<ParsedHttpMessage> function, ClientInfo info, ParsedHttpMessage parsedHttpMessage) 
			throws SQLException {
		
		if (function != null) {
			AddDataBase(parsedHttpMessage.getDatabaseName());
			function.apply(info, parsedHttpMessage);
		}else {
			SendResponse(info, HttpStatusCode.BAD_REQUEST, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, "No Function for Method"));
		}		
	}
		
	private class NotImplementedMethod implements DBFunction<ParsedHttpMessage>	{
		NotImplemented function = new NotImplemented();
		
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedHttpMessage) throws SQLException {
			function.apply(info, parsedHttpMessage);
		}
	}		
		
	private class OptionsMethod implements DBFunction<ParsedHttpMessage>	{
		OptionDisplay function = new OptionDisplay();
		
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedHttpMessage) throws SQLException {
			function.apply(info, parsedHttpMessage);
		}
	}		
		
	private class DeleteMethod implements DBFunction<ParsedHttpMessage>	{
		private HashMap<DatabaseKeys, DBFunction<ParsedHttpMessage>> functionMap = new HashMap<>();
		private DBFunction<ParsedHttpMessage> function;
		
		public DeleteMethod() {
			functionMap.put(DatabaseKeys.DELETE_ROW, new DeleteRow());
			functionMap.put(DatabaseKeys.DELETE_TABLE, new DeleteTable());
		}

		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedHttpMessage) throws SQLException {
			
			if ((null == parsedHttpMessage.getBodyMap()) && 
			    (parsedHttpMessage.getDatabaseKey() != DatabaseKeys.DELETE_TABLE)) {
				throw new NoSuchFieldError();
			}
			
			function = functionMap.get(parsedHttpMessage.getDatabaseKey());
			useFunction(function, info, parsedHttpMessage);
		}
	}		
		
	private class GetMethod implements DBFunction<ParsedHttpMessage>	{
		private HashMap<DatabaseKeys, DBFunction<ParsedHttpMessage>> functionMap = new HashMap<>();
		private DBFunction<ParsedHttpMessage> function;

		public GetMethod() {
			functionMap.put(DatabaseKeys.READ_ROW, new ReadRow());
			functionMap.put(DatabaseKeys.READ_FIELD_BY_NAME, new ReadFieldByName());
			functionMap.put(DatabaseKeys.READ_FIELD_BY_INDEX, new ReadFieldByIndex());
		}

		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedHttpMessage) throws SQLException {
			function = functionMap.get(parsedHttpMessage.getDatabaseKey());
			useFunction(function, info, parsedHttpMessage);
		}
	}
	
	private class PostMethod implements DBFunction<ParsedHttpMessage>	{
		private HashMap<DatabaseKeys, DBFunction<ParsedHttpMessage>> functionMap = new HashMap<>();
		private DBFunction<ParsedHttpMessage> function;

		public PostMethod() {
			functionMap.put(DatabaseKeys.CREATE_TABLE, new CreateTable());
			functionMap.put(DatabaseKeys.CREATE_IOT_EVENT, new CreateIOTEvent());
			functionMap.put(DatabaseKeys.CREATE_ROW, new CreateRow());
		}

		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedHttpMessage) throws SQLException {
			
			if (null == parsedHttpMessage.getBodyMap()) {
				throw new NoSuchFieldError();
			}
			
			function = functionMap.get(parsedHttpMessage.getDatabaseKey());
			useFunction(function, info, parsedHttpMessage);
		}
	}
	
	private class PutMethod implements DBFunction<ParsedHttpMessage>	{
		private HashMap<DatabaseKeys, DBFunction<ParsedHttpMessage>> functionMap = new HashMap<>();
		private DBFunction<ParsedHttpMessage> function;

		public PutMethod() {
			functionMap.put(DatabaseKeys.UPDATE_FIELD_BY_INDEX, new UpdateByIndex());
			functionMap.put(DatabaseKeys.UPDATE_FIELD_BY_NAME, new UpdateByName());
		}

		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedHttpMessage) throws SQLException {
			
			if (null == parsedHttpMessage.getBodyMap()) {
				throw new NoSuchFieldError();
			}
			
			function = functionMap.get(parsedHttpMessage.getDatabaseKey());
			useFunction(function, info, parsedHttpMessage);
		}
	}	
		
	//-----HTTP METHODS END -----//	
		
	//-----DB METHODS START -----//
		
	private class OptionDisplay implements DBFunction<ParsedHttpMessage>{

		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedHttpMessage) throws SQLException {
			responseHeader.put(HEADER_ALLOW, HEADER_ALLOWED_METHODS);
			SendResponse(info, HttpStatusCode.OK, HttpVersion.HTTP_1_1, null);
			responseHeader.remove(HEADER_ALLOW);
		}
			
	}
	
	private class NotImplemented implements DBFunction<ParsedHttpMessage>{
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedHttpMessage) throws SQLException {
			SendResponse(info, HttpStatusCode.NOT_IMPLEMENTED, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, "not implemented"));						
		}
		
	}
		
	private class ReadRow implements DBFunction<ParsedHttpMessage>{		
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			String DbName = parsedmsg.getDatabaseName();
				List<Object> returnVal = companyMap.get(DbName).readRow(
					parsedmsg.getTableName(),
					parsedmsg.getParamMap().get(URL_FIELD_PKNAME),
					AddCommas(parsedmsg.getParamMap().get(URL_FIELD_PKVALUE))
					);
			SendResponse(info, HttpStatusCode.OK, parsedmsg.getVersion() ,CreateMessageMap(returnVal, RESPONSE_ROW_VALUES));
		}
	}
	
	private class ReadFieldByName implements DBFunction<ParsedHttpMessage>{
		
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			Object returnVal = companyMap.get(parsedmsg.getDatabaseName()).readField(
					parsedmsg.getTableName(),
					parsedmsg.getParamMap().get(URL_FIELD_PKNAME),
					AddCommas(parsedmsg.getParamMap().get(URL_FIELD_PKVALUE)),
					parsedmsg.getParamMap().get(URL_FIELD_COLUMN_NAME)
					);
			SendResponse(info, HttpStatusCode.OK, parsedmsg.getVersion() ,CreateMessageMap(returnVal.toString(), RESPONSE_FIELD_VALUE));			
		}
	}		
		
	private class ReadFieldByIndex implements DBFunction<ParsedHttpMessage>{
		
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			Object returnVal = companyMap.get(parsedmsg.getDatabaseName()).readField(
					parsedmsg.getTableName(),
					parsedmsg.getParamMap().get(URL_FIELD_PKNAME),
					AddCommas(parsedmsg.getParamMap().get(URL_FIELD_PKVALUE)),
					toInt(parsedmsg.getParamMap().get(URL_FIELD_COLUMN_INDEX))
					);
			SendResponse(info, HttpStatusCode.OK, parsedmsg.getVersion() ,CreateMessageMap(returnVal.toString(), RESPONSE_FIELD_VALUE));			
		}
	}
	
	private class UpdateByIndex implements DBFunction<ParsedHttpMessage>{
		private static final String FIELD_UPDATED = "field updated";

		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			String DbName = parsedmsg.getDatabaseName();			
			
			companyMap.get(DbName).updateField(
					parsedmsg.getTableName(), 
					parsedmsg.getBodyMap().get(BODY_FIELD_PKNAME), 
					parsedmsg.getBodyMap().get(BODY_FIELD_PKVALUE),
					toInt(parsedmsg.getBodyMap().get(BODY_FIELD_COLUMN_INDEX)),
					parsedmsg.getBodyMap().get(BODY_FIELD_NEW_VALUE)
					);
			SendResponse(info, HttpStatusCode.OK, parsedmsg.getVersion() ,CreateMessageMap(RESPONSE_MESSAGE, FIELD_UPDATED));
		}
	}

	private class UpdateByName implements DBFunction<ParsedHttpMessage>{
		private static final String FIELD_UPDATED = "field updated";

		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			String DbName = parsedmsg.getDatabaseName();

			companyMap.get(DbName).updateField(
					parsedmsg.getTableName(), 
					parsedmsg.getBodyMap().get(BODY_FIELD_PKNAME), 
					parsedmsg.getBodyMap().get(BODY_FIELD_PKVALUE),
					parsedmsg.getBodyMap().get(BODY_FIELD_COLUMN_NAME),
					parsedmsg.getBodyMap().get(BODY_FIELD_NEW_VALUE)
					);
			SendResponse(info, HttpStatusCode.OK, parsedmsg.getVersion() ,CreateMessageMap(RESPONSE_MESSAGE, FIELD_UPDATED));
		}
	}
	
	private class DeleteRow implements DBFunction<ParsedHttpMessage>{
		private final static String ROW_DELETED = "row deleted";
	
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			companyMap.get(parsedmsg.getDatabaseName()).deleteRow(
					parsedmsg.getTableName(), 
					parsedmsg.getBodyMap().get(BODY_FIELD_PKNAME), 
					AddCommas(parsedmsg.getBodyMap().get(BODY_FIELD_PKVALUE))
					);
			SendResponse(info, HttpStatusCode.OK, parsedmsg.getVersion() ,CreateMessageMap(RESPONSE_MESSAGE,ROW_DELETED));	
		}
	}
	
	private class DeleteTable implements DBFunction<ParsedHttpMessage>{
		private final static String TABLE_DELETED = "table deleted";

		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			companyMap.get(parsedmsg.getDatabaseName()).deleteTable(parsedmsg.getTableName());
			SendResponse(info, HttpStatusCode.OK, parsedmsg.getVersion() ,CreateMessageMap(RESPONSE_MESSAGE,TABLE_DELETED));
		}
	}
	
	private class CreateTable implements DBFunction<ParsedHttpMessage>{
		private final static String TABLE_CREATED = "table created";
		
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			companyMap.get(parsedmsg.getDatabaseName()).createTable(parsedmsg.getBodyMap().get(BODY_FIELD_SQLCOMMAND));
			SendResponse(info, HttpStatusCode.CREATED, parsedmsg.getVersion() ,CreateMessageMap(RESPONSE_MESSAGE, TABLE_CREATED));
		}
	}
	
	private class CreateRow implements DBFunction<ParsedHttpMessage>{
		private final static String ROW_CREATED = "table created";
		
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			companyMap.get(parsedmsg.getDatabaseName()).createRow(parsedmsg.getBodyMap().get(BODY_FIELD_SQLCOMMAND));
			SendResponse(info, HttpStatusCode.CREATED, parsedmsg.getVersion() ,CreateMessageMap(RESPONSE_MESSAGE, ROW_CREATED));
		}
	}
	
	private class CreateIOTEvent implements DBFunction<ParsedHttpMessage>{
		private final static String IOT_UPDATED = "iot event added";
		
		@Override
		public void apply(ClientInfo info, ParsedHttpMessage parsedmsg) throws SQLException {
			String body = null;
			try {
				body = parsedmsg.getBodyMap().get(BODY_FIELD_RAW_DATA);
			} catch (NullPointerException e) {
				SendResponse(info, HttpStatusCode.BAD_REQUEST, HttpVersion.HTTP_1_1, CreateMessageMap(RESPONSE_ERRORMSG, "bad IOT input"));
			}
			if (body != null) {
				companyMap.get(parsedmsg.getDatabaseName()).createIOTEvent(body);
				SendResponse(info, HttpStatusCode.CREATED, parsedmsg.getVersion() ,CreateMessageMap(RESPONSE_MESSAGE, IOT_UPDATED));
			}
			
		}
	}
	
	//-----DB METHODS END -----//
	
	private int toInt(Object obj) {
		return Integer.parseInt(obj.toString());
	}
	
	private String AddCommas(String string) {
		return "'" + string + "'";
	}
	
	private void AddDataBase(String companyName) throws SQLException {
		if (!companyMap.containsKey(companyName)) {
			DatabaseManagement newDb = new DatabaseManagement(SERVER_URL, DB_USER, DB_PASS, companyName);
			companyMap.put(companyName, newDb);		
		}
	}
	
	//-----PACKAGE AND SEND METHODS START -----//
	
	private void SendResponse(ClientInfo info, HttpStatusCode status, HttpVersion version, HashMap<String, String> messageMap) {
		String body = null;
		int bodyLength = 0;
		if (messageMap != null) {
			body = gson.toJson(messageMap, messageMap.getClass()).replace("\\", "");
			bodyLength = body.length();
		}
		String httpResponse = HttpBuilder.createHttpResponseMessage(version, status, GetResponseHeader(bodyLength), body);
		System.out.println(httpResponse);
		SendMessage(httpResponse, info);
	}

	private Map<String, String> GetResponseHeader(int length) {
		responseHeader.put(HEADER_CONTENT_LENGTH, String.valueOf(length));
		
		return responseHeader;
	}

	private HashMap<String, String> CreateMessageMap(String key, String value) {
		responseMap.clear();
		responseMap.put(key, value);
		return responseMap;
	}
	
	private HashMap<String, String> CreateMessageMap(List<Object> list, String message) {
		responseMap.clear();
		responseMap.put(message, gson.toJson(list, list.getClass()));
		
		return responseMap;
	}
	
	private void SendMessage(String msg, ClientInfo info) {
		try {
			ByteBuffer httpBuffer = ByteBuffer.wrap(msg.getBytes(Charset.forName("UTF-8")));
			info.connection.sendMessage(httpBuffer, info);
		} catch (IOException e) {
			System.err.println("sending to client: " + info + "Failed");
		}		
	}
	
	//-----PACKAGE AND SEND METHODS END -----//

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
	private class ParsedHttpMessage{
		private ParsedURL parsedURL;
		private HashMap<String, String> bodyParams;

		private HttpVersion version;
		private HttpMethod method;

		
		@SuppressWarnings("unchecked")
		public ParsedHttpMessage(HttpParser msg) throws com.google.gson.stream.MalformedJsonException {
			parsedURL = new ParsedURL(msg.getStartLine().getURL());
			if (msg.getBody() != null) {
				bodyParams = gson.fromJson(msg.getBody().getBody(), HashMap.class);	
			}
			version = msg.getStartLine().getHttpVersion();
			method = msg.getStartLine().getHttpMethod();
		}

		public HttpMethod getHttpMethod() {
			return method;
		}	

		public DatabaseKeys getDatabaseKey() {
			return parsedURL.getDatabaseKey();
		}


		public String getDatabaseName() {
			return parsedURL.getDatabaseName();
		}
		
		public HttpVersion getVersion() {
			return version;
		}


		public String getTableName() {
			return parsedURL.getTableName();
		}
		public Map<String, String> getBodyMap() {
			return bodyParams;
		}

		public HashMap<String, String> getParamMap() {
			return parsedURL.getParamMap();
		}
		
		private DatabaseKeys GetDBKey(String dbString) {
			for (DatabaseKeys key : DatabaseKeys.values()) {
				if (key.toString().equals(dbString)) {
					return key;
				}
			}
			return null;
		}
		
		private class ParsedURL{
			private String DatabaseName;
			private String TableName;
			private DatabaseKeys databaseKey;
			private HashMap<String, String> paramMap = new HashMap<>();
			
			public ParsedURL(String url) {
				String[] questionMarkSeperation = url.split("\\?");
				String[] dbAndTable = questionMarkSeperation[0].split("\\/");
				
				if (questionMarkSeperation.length > 1) {
					String[] params = questionMarkSeperation[1].split("\\&");
					String[] paramParts;
					for (int i = 0; i < params.length; i++) {
						paramParts = params[i].split("=");
						paramMap.put(paramParts[0], paramParts[1]);
					}
				}
				try {
					DatabaseName = dbAndTable[1];
					TableName = dbAndTable[2];
					databaseKey = GetDBKey(dbAndTable[3]);
				} catch (ArrayIndexOutOfBoundsException e) {}
				
			}
			
			public String getDatabaseName() {
				return DatabaseName;
			}

			public String getTableName() {
				return TableName;
			}
			
			public DatabaseKeys getDatabaseKey() {
				return databaseKey;
			}

			public HashMap<String, String> getParamMap() {
				return paramMap;
			}
		}
	}
}
	

	public  interface DBFunction <U> {
		public void apply(ClientInfo info, U parsedHttpMessage) throws SQLException, NoSuchFieldError;
	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private HashMap<ProtocolType, Protocol> protocolMap = new HashMap<ProtocolType, Protocol>();;
		
		public MessageHandler() {
			protocolMap.put(ProtocolType.DB_HTTP, new DBHttpProtocol());
		}

		void handleMessage(ByteBuffer message, ClientInfo info) {

			ServerMessage msg;
			try {
				msg = (ServerMessage) ConversionUtils.toObject(message.array());
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


