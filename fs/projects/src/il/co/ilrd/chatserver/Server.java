package il.co.ilrd.chatserver;

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
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Set;


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
		public void handleMessage(ClientInfo info, GlobalMessage<?, ?> msg);
	}

	/**********************************************
	 * Chat Protocol
	 **********************************************/
	private class ChatProtocol implements Protocol{
		private HashMap<GlobalChatProtocolKeys, ChatFunction> funcionMap = new HashMap<>();
		private LinkedList<ChatClientInfo> userList = new LinkedList<>();
		private GlobalServerMessage defaultError = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, 
				new GlobalChatServerMessage(GlobalChatProtocolKeys.ERROR_MESSAGE, "User Does not exist"));
		
		private static final String USER_NOT_FOUND = "User Not Found";
		private static final String WRONG_KEY = "Key used is inaccurate";
		private final static String REG_FAILED = "Registration Failed";
		
		public ChatProtocol() {
			WrongHandler wrongHandler = new WrongHandler();
			funcionMap.put(GlobalChatProtocolKeys.REGISTRATION_REQUEST, new RegistrationRequest());
			funcionMap.put(GlobalChatProtocolKeys.MESSAGE, new SendingMessage());
			funcionMap.put(GlobalChatProtocolKeys.REMOVE_REQUEST, new RemoveRequest());
			funcionMap.put(GlobalChatProtocolKeys.REGISTRATION_ACK, wrongHandler);
			funcionMap.put(GlobalChatProtocolKeys.REGISTRATION_REFUSE, wrongHandler);
			funcionMap.put(GlobalChatProtocolKeys.NEW_CLIENT_REGISTRATION, wrongHandler);
			funcionMap.put(GlobalChatProtocolKeys.BROADCAST_MESSAGE, wrongHandler);
			funcionMap.put(GlobalChatProtocolKeys.ERROR_MESSAGE, wrongHandler);
		}
		
		@Override
		public void handleMessage(ClientInfo info, GlobalMessage<?, ?> msg) {
			
			if (info.connection.getPort() != GlobalProtocolPort.CHAT_PROTOCOL_PORT.getPort()) {
				System.err.println("Port/ProtocolKey combination is wrong");
				SetMessage(defaultError, "Wrong port used to call Chat. please use " + 
																		GlobalProtocolPort.CHAT_PROTOCOL_PORT.getPort() + "instead" + 
																		info.connection.getPort());
				SendMessage(defaultError, info);
				return;
			}
			
			funcionMap.get(((GlobalChatServerMessage)msg.getData()).getKey()).apply(info,(GlobalChatServerMessage)msg.getData());
		}
		
		private class RegistrationRequest implements ChatFunction{
			private GlobalServerMessage regAck = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, new GlobalChatServerMessage(GlobalChatProtocolKeys.REGISTRATION_ACK, ""));
			private GlobalServerMessage regRefuse = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, new GlobalChatServerMessage(GlobalChatProtocolKeys.REGISTRATION_REFUSE, REG_FAILED));
			private static final String HAS_JOINED = " Has Joined";

			@Override
			public void apply(ClientInfo info, GlobalChatServerMessage message) {
				String requestedUser =  message.getData();
				
				try {
					if (isUserAvailable(info, requestedUser)) {
						SetMessage(regAck, requestedUser);
						SendMessage(regAck, info);
						sendToAllUsers(new String().concat(requestedUser).concat(HAS_JOINED), 
										GlobalChatProtocolKeys.NEW_CLIENT_REGISTRATION);
						userList.add(new ChatClientInfo(info, requestedUser));
					}else {
						SendMessage(regRefuse, info);
					}
				} catch (IOException e) {
					System.err.println("put to Buffer failed");
				}
			}
		}
		
		private class SendingMessage implements ChatFunction{
			private GlobalServerMessage bcMsg = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, new GlobalChatServerMessage(GlobalChatProtocolKeys.BROADCAST_MESSAGE, ""));

			@Override
			public void apply(ClientInfo info, GlobalChatServerMessage message) {
				if (!isUserExists(info)) {
					SetMessage(defaultError, USER_NOT_FOUND);
					SendMessage(defaultError, info);
					return;
				}
				
				SetMessage(bcMsg, findChatClient(info).getUserName() + ": " + message.getData());
				for (ChatClientInfo chatClientInfo : userList) {
					if (info != chatClientInfo.clientInfo) {
						SendMessage(bcMsg, chatClientInfo.getServerClientInfo());
					}
				}
			}

			private boolean isUserExists(ClientInfo info) {
				for (ChatClientInfo chatClientInfo : userList) {
					if (chatClientInfo.getServerClientInfo().equals(info)) {
						return true;
					}
				}	
				return false;
			}
		}
		
		private class RemoveRequest implements ChatFunction{
			private static final String HAS_LEFT = " Has Left";
			
			@Override
			public void apply(ClientInfo info, GlobalChatServerMessage message) {
				
				ChatClientInfo chatClient = findChatClient(info);
				
				try {
					if (userList.remove(chatClient)) {
						sendToAllUsers(new String().concat(chatClient.getUserName()).concat(HAS_LEFT), 
										GlobalChatProtocolKeys.BROADCAST_MESSAGE);
					}else {
						SetMessage(defaultError, USER_NOT_FOUND);
						SendMessage(defaultError, info);
					}
				} catch (IOException e) {
					System.err.println("Sending remove message failed");
				}
			}
		}
		
		private class WrongHandler implements ChatFunction{
			@Override
			public void apply(ClientInfo info, GlobalChatServerMessage message) {
				SetMessage(defaultError, WRONG_KEY);
				SendMessage(defaultError, info);
			}
		}
		
		private boolean isUserAvailable(ClientInfo requestingClient, String requestedUser) {
			for (ChatClientInfo existingUser : userList) {
				if (existingUser.getUserName().equals(requestedUser) || 
					requestingClient.gettcpSocket().equals(existingUser.getServerClientInfo().gettcpSocket())) {
					return false;
				}
			}
			return true;
		}
		
		public void SetMessage(GlobalServerMessage dest, GlobalChatServerMessage src) {
			((GlobalChatServerMessage)dest.getData()).setData(src.getData());			
		}

		private void SetMessage(GlobalServerMessage dest, String src) {
			((GlobalChatServerMessage)dest.getData()).setData(src);
		}

		private void sendToAllUsers(String string, GlobalChatProtocolKeys key) 
				throws IOException {
			GlobalServerMessage notificationMsg = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, 
											new GlobalChatServerMessage(key, string));
			for (ChatClientInfo clientInfo : userList) {
				SendMessage(notificationMsg, clientInfo.getServerClientInfo());
			}
		}
		private void SendMessage(GlobalServerMessage notificationMsg, ClientInfo clientInfo) {
			ChatClientInfo user = findChatClient(clientInfo);
			if (!clientInfo.gettcpSocket().isConnected()) {
				System.out.println(user.getUserName() + " left the Chat");
				userList.remove(user);
			}
			try {
				buffer.clear();
				buffer.put(ObjectCovertUtil.toByteArray(notificationMsg));
				buffer.flip();
				clientInfo.connection.sendMessage(buffer, clientInfo);			
			} catch (IOException e) {
				System.err.println("Error sending message to " + user.getUserName());
			}
		}
		
		private ChatClientInfo findChatClient(ClientInfo info) {
			for (ChatClientInfo chatClientInfo : userList) {
				if (chatClientInfo.getServerClientInfo().equals(info)){
					return chatClientInfo;
				}
			}
			return null;
		}
		
		private class ChatClientInfo{
			private ClientInfo clientInfo;
			private String userName;
			
			public ChatClientInfo(ClientInfo clientInfo, String userName) {
				this.clientInfo = clientInfo;
				this.userName = userName;
			}
			
			public ClientInfo getServerClientInfo() {
				return clientInfo;
			}
	
			public String getUserName() {
				return userName;
			}
		}
	}
	
	private interface ChatFunction{
		public void apply(ClientInfo info, GlobalChatServerMessage message);
	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private HashMap<GlobalProtocolType, Protocol> protocolMap = new HashMap<GlobalProtocolType, Protocol>();;
		
		public MessageHandler() {
			protocolMap.put(GlobalProtocolType.CHAT_SERVER, new ChatProtocol());
		}

		void handleMessage(ByteBuffer message, ClientInfo info) {

			GlobalServerMessage msg;
			try {
				msg = (GlobalServerMessage) ObjectCovertUtil.toObject(message.array());
				System.out.println("msg recieved: "+msg);
				protocolMap.get(msg.getKey()).handleMessage(info, (GlobalMessage<?, ?>) msg);
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

