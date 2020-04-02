package il.co.ilrd.pingpong.handlers;

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
import java.util.Set;

public class Server implements Runnable{
	private static final int BUFFER_SIZE = 32768;
	private static final String KILL_STRING = "exit";
	private ByteBuffer buffer;
	
	private boolean started = false;
	private boolean killed = false;
	private ConnectionHandler connectHandler = new ConnectionHandler();
	private MessageHandler msgHandler = new MessageHandler();
	
	public Server() {
		buffer = ByteBuffer.allocate(BUFFER_SIZE);
	}

	@Override
	public void run() {
		try {
			startServer();
		} catch (IOException e) {
			e.getCause();
		}
	}
	
	public void startServer() 
			throws IOException {
		started = true;
		new Thread(new ExitServer()).start();
		connectHandler.startConnections();
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


	public void addTcpConnection(int portNumber, InetAddress address){
		checkIfStarted();
		connectHandler.addConnection(new TcpConnection(portNumber, address));
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
	class ConnectionHandler {
		private Selector selector;
		private LinkedList<Connection> connections = new LinkedList<Connection>();
		private HashMap<Channel, Connection> channelmap = new HashMap<Channel, Connection>();
		
		private void startConnections() throws IOException  {
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
					selector.select();
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
		public void sendMessage(ByteBuffer message, ClientInfo info);
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
		private InetAddress address;
		
		public TcpConnection(int port, InetAddress address)  {
			this.port = port;
			this.address = address;
		}

		@Override
		public void sendMessage(ByteBuffer message, ClientInfo info) {
			
			try {
				message.flip();
				info.tcpPath.write(message);
			} catch (IOException e) {
				System.err.println("Sending TCP message to " + info + "Failed");
			}
			finally {
				message.clear();
			}
		}

		@Override
		public void bufferToMsgHandler(SelectionKey key, Connection connection) {
			
			try {
				SocketChannel client = (SocketChannel) key.channel();

				if (-1 == client.read(buffer)) {
					client.close();
				}else {
					msgHandler.handleMessage(buffer, new ClientInfo(client,this));
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
				tcpSocket.bind(new InetSocketAddress(address, port));
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
			message.flip();
			try {
				udpSocket.send(message, info.udpPath);
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
	interface Protocol<K, V> {
		public void handleMessage(ClientInfo info, Message<?, ?> msg);
		public int getKey();
	}

	/**********************************************
	 * Ping Pong Protocol
	 **********************************************/

	private class PingPongProtocol<K, V> implements Protocol<K, V> {
		public static final int PING_PONG_KEY = 1;
		private static final String PING = "ping";
		private static final String PONG = "pong";
		private static final String BAD_INPUT = "nopet";
		
		private final PingPongMessage pingMsg = new PingPongMessage(PING_PONG_KEY, PING);
		private final PingPongMessage pongMsg = new PingPongMessage(PING_PONG_KEY, PONG);
		private final PingPongMessage badMsg = new PingPongMessage(PING_PONG_KEY, BAD_INPUT);
		
		@Override
		public void handleMessage(ClientInfo info, Message<?, ?> message)  {
			PingPongMessage msgToReturn;
			String pingOrPong = getPingOrPong(message);

			if (pingOrPong.equals(PING)) {
				msgToReturn = pingMsg;
			}else if (pingOrPong.equals(PONG)) {
				msgToReturn = pongMsg;
			}
			else {
				msgToReturn = badMsg;
			}
		
			try {
				info.connection.sendMessage(buffer.put(PingPongMessage.toByteArray(msgToReturn)), info);
			} catch (IOException e) {
				System.err.println("Message conversion failed");
			}

		}

		private String getPingOrPong(Message<?, ?> message) {
			String usrdata = ((String) message.getData().toString());
			
			if (usrdata.equals(PING)) {
				return PONG;
			}
			if (usrdata.equals(PONG)) {
				return PING;
			}
			return BAD_INPUT;
		}

		@Override
		public int getKey() {
			return PING_PONG_KEY;
		}
	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private HashMap<Integer, Protocol<?, ?>> protocolMap = new HashMap<Integer, Protocol<?,?>>();;
		
		public MessageHandler() {
			Protocol <?,?> pingPongProtocol = new PingPongProtocol<>();
			addProtocol(pingPongProtocol, pingPongProtocol.getKey());
		}

		void handleMessage(ByteBuffer message, ClientInfo info) {
			
			PingPongMessage msg;
			try {
				msg = (PingPongMessage) PingPongMessage.toObject(message.array());
				protocolMap.get(msg.getKey()).handleMessage(info, (Message<?, ?>) msg);
			} catch (ClassNotFoundException e) {
				System.err.println("Protocol Not Found");
				return;
			} catch (IOException e) {
				System.err.println("Message Decryption failed");
			}
		}

		private void addProtocol(Protocol<?, ?> protocol, int key) {
			protocolMap.put(key, protocol);
		}
	}
	/***********************************************
	 * Client info
	 **********************************************/
	private class ClientInfo{
		private SocketChannel tcpPath;
		private SocketAddress udpPath;
		private Connection connection;
		
		public ClientInfo(SocketAddress udpPath, Connection connection) {
			this.udpPath = udpPath;
			this.connection = connection;
		}
		
		public ClientInfo(SocketChannel tcpPath, Connection connection) {
			this.tcpPath = tcpPath;
			this.connection = connection;
		}

		@Override
		public String toString() {
			return "ClientInfo [tcpPath = " + tcpPath + ", udpPath = " + udpPath + ", connection = " + connection + "]";
		}
	}
}
