package il.co.ilrd.gatewayserver;


import static java.nio.file.StandardWatchEventKinds.ENTRY_MODIFY;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.InvocationTargetException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.Channel;
import java.nio.channels.ClosedSelectorException;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.file.ClosedWatchServiceException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.function.Consumer;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.JsonSyntaxException;
import com.google.gson.stream.JsonReader;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import il.co.ilrd.http_message.HttpBuilder;
import il.co.ilrd.http_message.HttpParser;
import il.co.ilrd.http_message.HttpStatusCode;
import il.co.ilrd.http_message.HttpVersion;
import il.co.ilrd.jarloader.JarLoader;
import il.co.ilrd.observer.Callback;
import il.co.ilrd.observer.Dispatcher;

public class GatewayServer implements Runnable{
	private static final String COMMAND_KEY = "CommandKey";
	private static final String DATA = "Data";
	private static final int BUFFER_SIZE = 32768;
	private static final int MAXIMUM_THREADS = 20;
	private static final int DEFAULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();
	private static final String DB_URL = "jdbc:mysql://127.0.0.1/";
	private static final String DB_USER = "root";
	private static final String DB_PASS = "root";
	private static final String DB_NAME = "dbName";
	private final String dirPath;
	private static final String FACTORY_COMMAND_MODIFIER = "FactoryCommandModifier";
	private JarMonitor jarMonitor;
	private FactoryCommandLoader loader;
	private ThreadPoolExecutor threadPool;
	private CMDFactory<FactoryCommand, String, Object> cmdFactory;
	private ConnectionsHandler connectionHandler;
	private GatewayMessageHandler messageHandler;
	
	private HashMap<String, DatabaseManagementInterface> dbMap = new HashMap<>();
	
	private GsonBuilder builder = new GsonBuilder();
	private Gson gson = builder.create();
	
	
	private boolean serverStopped = false;
	private boolean serverStarted = false;
	
	public GatewayServer(int numOfThreads, String dirPath) throws IOException {
		this.dirPath = dirPath;
		cmdFactory = CMDFactory.getFactory();

		connectionHandler = new ConnectionsHandler();
		messageHandler = new GatewayMessageHandler();
		
			threadPool = new ThreadPoolExecutor(
					numOfThreads, 
					MAXIMUM_THREADS, 
					1, TimeUnit.SECONDS, 
					new LinkedBlockingQueue<Runnable>());
		jarMonitor = new JarMonitor(dirPath);
		loader = new FactoryCommandLoader();
	}

	public GatewayServer(String dirPath) throws IOException {
		this(DEFAULT_NUM_THREADS, dirPath);
	}
	
	public void addHighHttpServer(ServerPort port) {
		checkIfServerStarted();
		connectionHandler.addConnection(new HighLevelHTTP(port));
	}
	
	public void addLowHttpServer(ServerPort port) {
		checkIfServerStarted();
		connectionHandler.addConnection(new LowLevelHTTP(port));
	}

	public void addTcpServer(ServerPort port) {
		checkIfServerStarted();
		connectionHandler.addConnection(new TcpConnection(port));
	}
	
	public void addUdpServer(ServerPort port) {
		checkIfServerStarted();
		connectionHandler.addConnection(new UdpConnection(port));
	}
	
	public void start() {
		checkIfServerStarted();
		new Thread(jarMonitor).start();
		serverStarted = true;
		try {
			connectionHandler.startConnections();
		} catch (IOException e) {
			System.err.println("Starting connections failed");
			e.printStackTrace();
		}
	}
	
	@Override
	public void run() {
		start();
	}
	
	public void stop() {
		if (serverStarted) {
			serverStopped = true;
			System.out.println("IN STOP");
			try {
				connectionHandler.stopConnections();
				threadPool.shutdown();
				jarMonitor.stopUpdate();
			} catch (IOException e) {
				System.err.println("stopping server failed");
				e.printStackTrace();
			}			
		}
	}
	
	public void setNumOfThreads(int numOfThread) {
		threadPool.setCorePoolSize(numOfThread);
	}
	
	private void checkIfServerStarted() {
		if (serverStarted) {
			throw new ExceptionInInitializerError("Server Already started");
		}
	}
	
	private class ConnectionsHandler {
		private Selector selector;
		private LinkedList<ServerConnection> connections = new LinkedList<>();
		private HashMap<Channel, ServerConnection> channelmap = new HashMap<>();
		private static final long TIMEOUT_PRINT = 20000;
		
		private void startConnections() throws IOException  {

			try {
				selector = Selector.open();
			} catch (IOException e) {
				System.err.println("Selector open failed");
			}
			System.out.println("selector opened");
			
			for (ServerConnection serverConnection : connections) {
				serverConnection.initServerConnection(channelmap, selector);
			}
			System.out.println("finished init connection handler");

			try {
				while (true) {
					while (0 == selector.select(TIMEOUT_PRINT)) {
						System.out.println("Server is waiting");
					}
					Set<SelectionKey> selectedKeys = selector.selectedKeys();
					Iterator<SelectionKey> iter = selectedKeys.iterator();
					
					while (iter.hasNext()) {
						SelectionKey key = iter.next();
						if (key.isValid() && key.channel().isOpen()) {
							ServerConnection connection = channelmap.get(key.channel());
							if (key.isAcceptable() ) {
								registerTcpClient(key.channel(),connection);
							}
							if (key.isReadable()) {
								connection.handleRequestMessage(key, connection);
							}
						}
						iter.remove();
					}
				}
			} catch (ClosedSelectorException e) {
				if (serverStopped ) {					
					System.out.println("Server Exiting");
					return;
				}
			}
		}
		
		private void addConnection(ServerConnection connection) {
			connections.add(connection);
		}

		private void registerTcpClient(Channel channel, ServerConnection connection) {
			SocketChannel tcpClient; 
			
			try {
				tcpClient = ((ServerSocketChannel) channel).accept();
				tcpClient.configureBlocking(false);
				tcpClient.register(selector, SelectionKey.OP_READ);
				channelmap.put(tcpClient, connection);
				connection.getClientMap().put(tcpClient, new ClientInfo(tcpClient, connection));
			} catch (IOException e) {
				System.err.println("TCP Register failed");
				e.printStackTrace();
			}
		}
	
		private void stopConnections() throws IOException {
			for (ServerConnection serverConnection : connections) {
				serverConnection.stopServer();
			}
			if (selector != null) {
				selector.close();
			}
		}
	}
	
	public class JarMonitor implements DirMonitor, Runnable {
		private Dispatcher<String> dispatcher = new Dispatcher<>();
		private WatchService watcher = FileSystems.getDefault().newWatchService();
		
		public JarMonitor(String dirPath) throws IOException {
			initWatchService(dirPath);
		}
		
		@Override
		public void stopUpdate() throws IOException {
			dispatcher.stopUpdate();
			watcher.close();
			System.out.println("stopped");
		}

		@Override
		public void register(Callback<String> callback) {
			dispatcher.register(callback);
			System.out.println("Register");
		}

		@Override
		public void unregister(Callback<String> callback) {
			dispatcher.unregister(callback);
		}
		
		private void update(String path) {
			dispatcher.updateAll(path);
		}
		
		private void initWatchService(String filePath) throws IOException {
			Path dir = FileSystems.getDefault().getPath(filePath);	
			watcher = FileSystems.getDefault().newWatchService();
	        dir.register(watcher, ENTRY_MODIFY);
	    
	        System.out.println("file to watch: " + dir);
		}
		
		@Override
		public void run() {
			System.out.println("STARTING RUN");
			Path path;
			WatchKey key;
	        while (!serverStopped) {
			    try {
					key = watcher.take();
				    if (null != key) {
				    	 for (WatchEvent<?> event : key.pollEvents()) {
				    		 path = (Path)event.context();
				    		if (path.getFileName().toString().endsWith(".jar")) {
				    			path = (Path) key.watchable();
				    			update(path.resolve((Path) event.context()).toString());
							}
				    	 }
						 key.reset();
					}
				}catch (InterruptedException e ) {
					e.printStackTrace();
				}catch (ClosedWatchServiceException e) {
					//this exception caught to interrupt the blocking watcher.take() in case of closing.
				}
			}
	    }
	}
	
	private class FactoryCommandLoader{
		Map<String, Integer> versionLookup = new HashMap<>();
		String curCommand;
		int curVersion;
		private void loader(String string) {
			try {
				List<Class<?>> classList = JarLoader.load(FACTORY_COMMAND_MODIFIER, string);
				for (Class<?> curClass : classList) {
					FactoryCommandModifier cmf = (FactoryCommandModifier) curClass.getConstructor().newInstance();
					curCommand = cmf.getCommand();
					curVersion = cmf.getVersion();					
					if (!versionLookup.containsKey(curCommand) || 
					   (versionLookup.get(curCommand) > curVersion)) {
						versionLookup.put(curCommand, curVersion);
						cmf.addToFactory();
					}
				}
			} catch (InstantiationException | 
					SecurityException | 
					ClassNotFoundException | 
					IOException | 
					IllegalAccessException | 
					IllegalArgumentException | 
					InvocationTargetException |
					NoClassDefFoundError |
					NoSuchMethodException e ) {
				System.err.println("error reading jar");
				e.printStackTrace();
			}
		}

		private Callback<String> callback = new Callback<String>((param)-> loader(param), null);
		
		public FactoryCommandLoader() {
			initFactory();
			jarMonitor.register(callback);
		}

		private void initFactory() {
			for (File curFile : new File(dirPath).listFiles()) {
				if (curFile.getName().endsWith(".jar")) {
						List<Class<?>> classList;
						try {
							classList = JarLoader.load(FACTORY_COMMAND_MODIFIER, curFile.getAbsolutePath());
							for (Class<?> curClass : classList) {
								
								FactoryCommandModifier cmf = (FactoryCommandModifier) curClass.getConstructor().newInstance();
								
								versionLookup.put(cmf.getCommand(), cmf.getVersion());
								cmf.addToFactory();
							}
						} catch (ClassNotFoundException | IOException | InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException | NoSuchMethodException | SecurityException e) {
							e.printStackTrace();
						}
				}
			}			
		}
	}
			
	
	private interface ServerConnection {
		public void initServerConnection(HashMap<Channel, ServerConnection> channelmap, Selector selector) throws IOException;
		public void handleRequestMessage(SelectionKey key, ServerConnection connection);
		public void sendResponse(ByteBuffer message, ClientInfo info);
		public void stopServer() throws IOException;
		public Map<SocketChannel, ClientInfo> getClientMap();
	}
	
	private class LowLevelHTTP implements ServerConnection{
		private int port;
		private ServerSocketChannel tcpSocket;
		private HashMap<String, String> responseHeader = new HashMap<>();
		private Map<SocketChannel, ClientInfo> clientMap = new HashMap<>();
		
		public LowLevelHTTP(ServerPort port) {
			this.port = port.getPort();
		}
		
		@Override
		public void initServerConnection(HashMap<Channel, ServerConnection> channelmap, Selector selector) {
			System.out.println("init http lop on port " + port);
			try {
				tcpSocket = ServerSocketChannel.open();
				tcpSocket.configureBlocking(false);
				tcpSocket.bind(new InetSocketAddress(port));
				tcpSocket.register(selector, SelectionKey.OP_ACCEPT);	
				channelmap.put(tcpSocket, this);
			} catch (IOException e) {
				System.err.println("TCP on port "+ port + "failed");
				e.printStackTrace();
			}			
		}
		
		@Override
		public void handleRequestMessage(SelectionKey key, ServerConnection connection) {
			TcpConnection tcpConnection = (TcpConnection) connection;
			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
			try {
				SocketChannel client = (SocketChannel) key.channel();

				if (!(-1 == client.read(buffer))) {
					HttpParser parser = new HttpParser(bufferToString(buffer));
					messageHandler.handleMessage(stringToBuffer(parser.getBody().getBody()), tcpConnection.socketInfo.get(client));
				}else {
					client.close();
				}
			} catch (IOException e) {
				System.err.println("Connection closed by client");
			}finally {
				buffer.clear();
			}
		}

		@Override
		public void sendResponse(ByteBuffer message, ClientInfo info) {
			String body = bufferToString(message);
			try {
				String response = HttpBuilder.createHttpResponseMessage(
						HttpVersion.HTTP_1_1, 
						HttpStatusCode.OK, 
						GetResponseHeader(body.length()),
						body);
				
				info.getTcpSocket().write(stringToBuffer(response));
				message.clear();
			} catch (IOException e) {
				System.err.println("Sending response " + body + " failed");
				e.printStackTrace();
			}
		}
		
		@Override
		public String toString() {
			return "HttpConnection [" + tcpSocket + "]" ;
		}

		@Override
		public void stopServer() throws IOException {
			try {
				tcpSocket.close();
			} catch (NullPointerException e) {}
		}
		
		private Map<String, String> GetResponseHeader(int length) {
			responseHeader.put("Content-Length", String.valueOf(length));
			return responseHeader;
		}

		@Override
		public Map<SocketChannel, ClientInfo> getClientMap() {
			return clientMap;
		}
	}
	
	private class HighLevelHTTP implements ServerConnection{
		private HttpServer httpServer;
		private int port;
		private final static String CONTENT_TYPE = "Content-Type";
		private final static String CONTENT_JSON = "application/json";

		public HighLevelHTTP(ServerPort port) {
			this.port = port.getPort();
		}

		@Override
		public void initServerConnection(HashMap<Channel, ServerConnection> channelmap, Selector selector) throws IOException {
			httpServer = HttpServer.create(new InetSocketAddress(port), 0);
			httpServer.createContext("/", 
					new HttpHandler() {
						@Override
						public void handle(HttpExchange msg) throws IOException {
							System.out.println(msg.getRemoteAddress());
							ByteBuffer buffer = ByteBuffer.wrap(msg.getRequestBody().readAllBytes());
							messageHandler.handleMessage(buffer, new ClientInfo(msg, HighLevelHTTP.this));					
						}
					}
				);
			httpServer.start();
		}

		@Override
		public void sendResponse(ByteBuffer message, ClientInfo info) {
			try {
				info.exchangeMsg.getResponseHeaders().add(CONTENT_TYPE, CONTENT_JSON);
				info.exchangeMsg.sendResponseHeaders(HttpStatusCode.OK.getCode(), bufferToString(message).length());
				OutputStream outStream = info.exchangeMsg.getResponseBody();
				outStream.write(message.array());
				outStream.close();
			} catch (IOException e) {
				System.err.println("sending response failed");
				e.printStackTrace();
			}
		}

		@Override
		public void handleRequestMessage(SelectionKey key, ServerConnection connection) {}

		@Override
		public void stopServer() {
			if (httpServer != null) {
				httpServer.stop(1);

			}
		}

		@Override
		public Map<SocketChannel, ClientInfo> getClientMap() {return null;}
	}
	
	private class UdpConnection implements ServerConnection{
		private DatagramChannel udpSocket;
		private int port;
		
		public UdpConnection(ServerPort port) {
			this.port = port.getPort();
		}

		@Override
		public void initServerConnection(HashMap<Channel, ServerConnection> channelmap, Selector selector)
				throws IOException {
			try {
				udpSocket = DatagramChannel.open();
				udpSocket.configureBlocking(false);
				udpSocket.socket().bind(new InetSocketAddress(port));
				udpSocket.register(selector, SelectionKey.OP_READ);
				channelmap.put(udpSocket, this);
			} catch (IOException e) {
				System.err.println("UDP on port "+ port + "failed");
			}
		}

		@Override
		public void sendResponse(ByteBuffer message, ClientInfo info) {
			try {
				udpSocket.send(message, info.getudpSocket());
			} catch (IOException e) {
				System.err.println("UDP to " + info + "Failed");
			}
		}

		@Override
		public void handleRequestMessage(SelectionKey key, ServerConnection connection) {
			SocketAddress clientAddress;
			try {
				ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
				clientAddress = udpSocket.receive(buffer);
				messageHandler.handleMessage(buffer, new ClientInfo(clientAddress, connection));
				buffer.clear();
			} catch (IOException e) {
				System.err.println("receiving UDP message failed");
			}
			
		}

		@Override
		public void stopServer() throws IOException {
			udpSocket.close();
			
		}
		
		@Override
		public Map<SocketChannel, ClientInfo> getClientMap() {return null;}
		
	}
	
	private class TcpConnection implements ServerConnection{
		private ServerSocketChannel tcpSocket;
		private int port;
		private Map<SocketChannel, ClientInfo> socketInfo = new HashMap<>();

		public TcpConnection(ServerPort port) {
			this.port = port.getPort();
		}

		@Override
		public void initServerConnection(HashMap<Channel, ServerConnection> channelmap, Selector selector) {
			System.out.println("init tcp connection on port " + port);
			try {
				tcpSocket = ServerSocketChannel.open();
				tcpSocket.configureBlocking(false);
				tcpSocket.bind(new InetSocketAddress(port));
				tcpSocket.register(selector, SelectionKey.OP_ACCEPT);	
				channelmap.put(tcpSocket, this);
			} catch (IOException e) {
				System.err.println("TCP on port "+ port + "failed");
				e.printStackTrace();
			}			
		}

		@Override
		public void sendResponse(ByteBuffer message, ClientInfo info) {
				try {
					info.getTcpSocket().write(message);
					message.clear();
				} catch (IOException e) {
					e.printStackTrace();
				}
		}

		@Override
		public void handleRequestMessage(SelectionKey key, ServerConnection connection) {
			TcpConnection tcpConnection = (TcpConnection) connection;
			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
			try {
				SocketChannel client = (SocketChannel) key.channel();

				if (!(-1 == client.read(buffer))) {
					messageHandler.handleMessage(buffer, tcpConnection.socketInfo.get(client));
				}else {
					client.close();
				}
			} catch (IOException e) {
				System.err.println("Connection closed by client");
			}finally {
				buffer.clear();
			}			
		}

		@Override
		public void stopServer() throws IOException {
			try {
				tcpSocket.close();
				
			} catch (NullPointerException e) {}
		}

		@Override
		public Map<SocketChannel, ClientInfo> getClientMap() {
			return socketInfo;
		}
		
	}
	
	private class ClientInfo{

		private SocketChannel tcpSocket;
		private SocketAddress udpSocket;
		private ServerConnection connection;
		private HttpExchange exchangeMsg;
		
		public SocketChannel getTcpSocket() {
			return tcpSocket;
		}

		public SocketAddress getudpSocket() {
			return udpSocket;
		}

		public ClientInfo(SocketAddress udpSocket, ServerConnection connection) {
			this.udpSocket = udpSocket;
			this.connection = connection;
		}
		
		public ClientInfo(SocketChannel tcpSocket, ServerConnection connection) {
			this.tcpSocket = tcpSocket;
			this.connection = connection;
		}

		public ClientInfo(HttpExchange msg, ServerConnection connection) {
			this.connection = connection;
			exchangeMsg = msg;
		}

		@Override
		public String toString() {
			return "ClientInfo [tcpSocket = " + tcpSocket + ", udpSocket = " + udpSocket + ", connection = " + connection + "]";
		}
	}
	
	private class GatewayMessageHandler{
		HashMap<String, String> json = new HashMap<>();
		JsonReader reader;
		
		public void handleMessage(ByteBuffer buffer, ClientInfo clientInfo) {
				try {
					threadPool.execute(convertToRunnable(bufferToJson(buffer), clientInfo));
				} catch (JsonSyntaxException e) {
					e.printStackTrace();
					sendStringResponse(getJsonFormat("Error", "Bad JSON Format"), clientInfo);
					System.err.println("bad json format");
				}
		}
		
		private JsonObject bufferToJson(ByteBuffer buffer) throws JsonSyntaxException{
			reader = new JsonReader(new StringReader(bufferToString(buffer)));
			reader.setLenient(true);
			JsonObject j = gson.fromJson(reader, JsonObject.class);
			return j;
		}
	}
	
	private Runnable convertToRunnable(JsonObject param, ClientInfo clientInfo) {
		String key = param.get(COMMAND_KEY).getAsString();

		JsonObject innerMessage = gson.fromJson(param.get(DATA), JsonObject.class);
		return new Runnable() {
			@Override
			public void run() {
					if (null != key) {
						String response = null;
						try {
							if (cmdFactory.map.containsKey(key)) {
									response = cmdFactory.create(key).run(innerMessage, getDatabase(innerMessage.get(DB_NAME).getAsString()));
									sendResponseFromTask(response, clientInfo);
							}else {
								sendStringResponse(getJsonFormat("Error", "Wrong " + COMMAND_KEY + " used"), clientInfo);								
							}
						} catch (SQLException e) {
							sendStringResponse(getJsonFormat("Error", e.getMessage()), clientInfo);								
						}															
					}else {
						sendStringResponse(getJsonFormat("Error", "Wrong key used"), clientInfo);
					}
			}

			private void sendResponseFromTask(String response, ClientInfo clientInfo) {
				if (null != response) {
					sendStringResponse(response, clientInfo);
				}
			}
		};
		
	}
	
	private String bufferToString(ByteBuffer buffer) {
		try {
			return new String(buffer.array(), "UTF-8");
		} catch (UnsupportedEncodingException e) {
			try {
				return new String(buffer.array(), "ASCII");
			} catch (UnsupportedEncodingException e1) {
				e1.printStackTrace();
				return null;
			}
		}
	}
	
	private ByteBuffer stringToBuffer(String string) {
		return ByteBuffer.wrap(string.getBytes(Charset.forName("UTF-8")));
	}
	
	private void sendStringResponse(String response, ClientInfo clientInfo) {
		clientInfo.connection.sendResponse(stringToBuffer(response), clientInfo);		
	}	
	
	private String getJsonFormat(String key, String value) {
		HashMap<String, String> hashMap = new HashMap<String, String>();
		hashMap.put(key, value);
		return gson.toJson(hashMap, hashMap.getClass());
	}

	private DatabaseManagementInterface getDatabase(String dbName) throws SQLException {
		DatabaseManagementInterface dbToReturn = dbMap.get(dbName);
		if (null == dbToReturn) {
			dbToReturn = new DatabaseManagement(DB_URL, DB_USER, DB_PASS, dbName);
			dbMap.put(dbName, dbToReturn);
		}
		
		return dbToReturn;
	}

	private class DatabaseManagement implements DatabaseManagementInterface{
		private final String databaseUrl;
		private final String userName;
		private final String password;
		private static final String SSL_OFF = "?useSSL=false";
		private static final String RAWDATA_DELIMETER = "\\|";
		private static final String COMMAND_DELIMETER = ";";
		private static final String RAWDATA_TABLE_NAME = " IOTEvent ";
		
		public DatabaseManagement(String url, String userName, String password, 
													String databaseName) throws SQLException {
			this.userName = userName;
			this.password = password;	
			this.databaseUrl = url + databaseName + SSL_OFF;
			createDatabase(databaseName, url + SSL_OFF);
		}
		
		@Override
		public void createIOTEvent(String rawData) throws SQLException { 
			String[] values = rawData.split(RAWDATA_DELIMETER);
			String sqlCommand = "INSERT INTO " + RAWDATA_TABLE_NAME +" VALUES (";
			int length = values.length;
			sqlCommand += "null, ";
			for (int i = 0; i < length; ++i) {
				if (i + 1 == length) {
					sqlCommand +=  values[i] + ")"; 
				} else {
					sqlCommand +=  values[i] + ", "; 
				}
			}
			executeSqlCommand(sqlCommand);
		}
		
		private void createDatabase(String databaseName, String url) throws SQLException {
			try(
				java.sql.Connection conn = DriverManager.getConnection(url, userName, password);
				Statement statement = conn.createStatement();
			){
				statement.execute(" CREATE DATABASE IF NOT EXISTS " + databaseName);
			} 
		}
				
		private void executeSqlCommand(String sqlCommands) throws SQLException {
			try(
					java.sql.Connection conn = DriverManager.getConnection(databaseUrl, userName, password);
					Statement statement = conn.createStatement();
			){
				String[] sqlCommandsArr = sqlCommands.split(COMMAND_DELIMETER);
				conn.setAutoCommit(false);	
				for (String command : sqlCommandsArr) {
					statement.addBatch(command.trim());			
				}
				statement.executeBatch();
				conn.commit();
			} 	
		}

		@Override
		public void createTable(String sqlCommand) throws SQLException {
			executeSqlCommand(sqlCommand);
			
		}

		@Override
		public void createRow(String sqlCommand) throws SQLException {
			executeSqlCommand(sqlCommand);
		}
	}
}