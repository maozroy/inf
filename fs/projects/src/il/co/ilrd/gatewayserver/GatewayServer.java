package il.co.ilrd.gatewayserver;

import java.io.IOException;
import java.io.OutputStream;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
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
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;
import com.google.gson.stream.JsonReader;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import il.co.ilrd.http_message.HttpBuilder;
import il.co.ilrd.http_message.HttpParser;
import il.co.ilrd.http_message.HttpStatusCode;
import il.co.ilrd.http_message.HttpVersion;

public class GatewayServer implements Runnable{
	private static final String COMMAND_KEY = "CommandKey";
	private static final String DATA = "Data";
	private static final int BUFFER_SIZE = 32768;
	private static final int MAXIMUM_THREADS = 20;
	private static final int DEFAULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();


	private ThreadPoolExecutor threadPool;
	private CMDFactory<FactoryCommand, CommandKey, Object> cmdFactory;
	private ConnectionsHandler connectionHandler;
	private GatewayMessageHandler messageHandler;
	
	private GsonBuilder builder = new GsonBuilder();
	private Gson gson = builder.create();
	
	private boolean serverStopped = false;
	private boolean serverStarted = false;
	
	public GatewayServer(int numOfThreads) {
		cmdFactory = new CMDFactory<>();
		connectionHandler = new ConnectionsHandler();
		messageHandler = new GatewayMessageHandler();
		
			threadPool = new ThreadPoolExecutor(
					numOfThreads, 
					MAXIMUM_THREADS, 
					1, TimeUnit.SECONDS, 
					new LinkedBlockingQueue<Runnable>());
		
		initFactory();
	}
	
	private void initFactory() {
		Function<Object, FactoryCommand> companyRegFunc = (Object a)-> new CompanyRegister();
		cmdFactory.add(CommandKey.COMPANY_REGISTRATION, companyRegFunc);
	}
	
	public GatewayServer() {
		this(DEFAULT_NUM_THREADS);
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
			} catch (IOException e) {
				System.err.println("stopping server failed");
				e.printStackTrace();
			}			
		}
	}
	
	public void setNumOfThreads(int numOfThread) {
		System.out.println(threadPool.getCorePoolSize());
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
			
			for (ServerConnection serverConnection : connections) {
				serverConnection.initServerConnection(channelmap, selector);
			}

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
			selector.close();
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
			httpServer.stop(1);
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
				
			} catch (NullPointerException e) {
				// TODO: handle exception
			}
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
					threadPool.execute(convertToRunnable(bufferToMap(buffer), clientInfo));
				} catch (JsonSyntaxException e) {
					sendStringResponse(getJsonFormat("Error", "Bad JSON Format"), clientInfo);
					System.err.println("bad json format");
				}
		}
		
		private HashMap<String, String> bufferToMap(ByteBuffer buffer) throws JsonSyntaxException{
			json.clear();
			reader = new JsonReader(new StringReader(bufferToString(buffer)));
			reader.setLenient(true);
			json = gson.fromJson(reader, HashMap.class);	
			return json;
		}
	}
	
	private Runnable convertToRunnable(HashMap<String, String> param, ClientInfo clientInfo) {
		CommandKey key = getCommandKey(param.get(COMMAND_KEY));
		
		return new Runnable() {
			CommandKey commandKey = key;
			Object data = param.get(DATA);
			@Override
			public void run() {
				if (null != key) {
					cmdFactory.create(commandKey).run(data, clientInfo);					
				}else {
					sendStringResponse(getJsonFormat("Error", "Wrong key used"), clientInfo);
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

	private CommandKey getCommandKey(String key) {
		for (CommandKey iterable_element : CommandKey.values()) {
			if (iterable_element.name().equals(key)) {
				return iterable_element;
			}
		}
		return null;
	}
	
	
	private String getJsonFormat(String key, String value) {
		HashMap<String, String> hashMap = new HashMap<String, String>();
		hashMap.put(key, value);
		return gson.toJson(hashMap, hashMap.getClass());
	}

	
 	private interface FactoryCommand {
		public void run(Object data, ClientInfo clientInfo);
	}
	
	private class CompanyRegister implements FactoryCommand {

		@Override
		public void run(Object data, ClientInfo clientInfo) {
			System.out.println("inside CompanyRegister");

			sendStringResponse(getJsonFormat("Response", "Register Succsess on" + data), clientInfo);
		}
	}


}