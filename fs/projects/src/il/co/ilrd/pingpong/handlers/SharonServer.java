package il.co.ilrd.pingpong.handlers;

import java.io.BufferedReader;
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
import java.util.List;
import java.util.Map;
import java.util.Set;

import il.co.ilrd.pingpong.handlers.PingPongMessage.InnerMessage;

public class SharonServer implements Runnable{
	private ConnectionHandler connectionHandler = new ConnectionHandler();
	private MessageHandler messageHandler = new MessageHandler();
	private boolean isRunning = false;
	private static final String EXIT = "exit";
	
	public void startServer() {
		isRunning = true;
		InputDetector exitDetector = new InputDetector();
		new Thread(exitDetector).start();
		try {
			connectionHandler.startConnections();
		} catch (ClosedSelectorException e1) {
			return;
		} catch (ClassNotFoundException | IOException e) {
			e.printStackTrace();
		}	
	}

	public void stopServer() {
		isRunning = false;
		
		try {
			connectionHandler.stopConnections();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void addTcpConnection(int portNumber) throws UnknownHostException, IOException {
		if(true == isRunning) {
			throw new AddConnectionException();
		}
		
		Connection tcpConnection = new TcpConnection(portNumber);
		connectionHandler.addConnection(tcpConnection);
	}

	public void addUdpConnection(int portNumber) throws IOException {
		if(true == isRunning) {
			throw new AddConnectionException();
		}
		
		Connection udpConnection = new UdpConnection(portNumber);
		connectionHandler.addConnection(udpConnection);
	}
	
	public void addBroadcastConnection(int portNumber) throws IOException {
		if(true == isRunning) {
			throw new AddConnectionException();
		}
		
		Connection broadcastConnection = new BroadcastConnection(portNumber);
		connectionHandler.addConnection(broadcastConnection);
	}
	
	@Override
	public void run() {
		startServer();
	}

/**********************************Connection Interface**************************************************/

	private interface Connection {
		public void sendMessage(ClientInfo clientInfo, ByteBuffer buffer) throws IOException;
		public void communicateWithClient(Channel clientChannel, ByteBuffer buffer) throws IOException, ClassNotFoundException;
		public void initConnection(Selector selector, int portNumber) throws IOException;
		public int getPortNumber();
		public Channel getSocket();
	}

/******************************TCP Connection********************************************************/
	private class TcpConnection implements Connection {
		private final int portNumber;
		private ServerSocketChannel tcpSocket;
		
		public TcpConnection(int portNumber) {
			this.portNumber = portNumber;
		}
		
		public void communicateWithClient(Channel clientChannel, ByteBuffer buffer) throws IOException, ClassNotFoundException {
			int bytes = ((SocketChannel)clientChannel).read(buffer);
	    	if (-1 == bytes) {
	    		clientChannel.close();
	    	}
	    	
	    	else {
	    		ClientInfo clientInfo = new ClientInfo(clientChannel);
	    		messageHandler.handleMessage(buffer, clientInfo);
	    	}
		}
	
		@Override
		public void sendMessage(ClientInfo clientInfo, ByteBuffer buffer) throws IOException {
			while (buffer.hasRemaining()) {
				((SocketChannel)clientInfo.clientChannel).write(buffer);
			}
			
			buffer.clear();
		}
		
		@Override
		public void initConnection(Selector selector, int portNumber) throws IOException {
			tcpSocket = ServerSocketChannel.open();
			tcpSocket.bind(new InetSocketAddress(InetAddress.getLocalHost(), portNumber));
			tcpSocket.configureBlocking(false);
			tcpSocket.register(selector, SelectionKey.OP_ACCEPT);
		}
		
		@Override
		public int getPortNumber() {
			return portNumber;
		}

		@Override
		public ServerSocketChannel getSocket() {
			return tcpSocket;
		}
	}

/*****************************UDP Connection*******************************************************/
	private class UdpConnection implements Connection {
		private final int portNumber;
		private DatagramChannel udpSocket;
		
		public UdpConnection(int portNumber) {
			this.portNumber = portNumber;
		}
		
		public void communicateWithClient(Channel clientChannel, ByteBuffer buffer) throws IOException, ClassNotFoundException {
			SocketAddress clientAddress = ((DatagramChannel)clientChannel).receive(buffer);
			ClientInfo clientInfo = new ClientInfo(clientChannel, clientAddress);
		    messageHandler.handleMessage(buffer, clientInfo);
		}
		
		@Override
		public void sendMessage(ClientInfo clientInfo, ByteBuffer buffer) throws IOException {
			udpSocket.send(buffer, clientInfo.udpClientAddress);
			buffer.clear();
		}
		
		@Override
		public void initConnection(Selector selector, int portNumber) throws IOException {
			udpSocket = DatagramChannel.open();
			udpSocket.socket().bind(new InetSocketAddress(portNumber));
			udpSocket.configureBlocking(false);
			udpSocket.register(selector, SelectionKey.OP_READ);
		}
		
		@Override
		public int getPortNumber() {
			return portNumber;
		}

		@Override
		public DatagramChannel getSocket() {
			return udpSocket;
		}
	}

/***************************Broadcast Connection***********************************************/
	private class BroadcastConnection extends UdpConnection {
		
		public BroadcastConnection(int portNumber) {
			super(portNumber);
		}
	}

/********************************** Connection Handler*****************************************/
	private class ConnectionHandler {
		private Selector selector;
		private List<Connection> connectionsList = new LinkedList<>();
		private HashMap<Channel, Connection> channelConnectionsMap = new HashMap<>();
		private ByteBuffer buffer;
		private static final int BUFFER_SIZE = 1024;
		private static final int TIME_TO_WAIT = 10000;
		
		private void startConnections() throws IOException, ClassNotFoundException, ClosedSelectorException {
			buffer = ByteBuffer.allocate(BUFFER_SIZE);
			selector = Selector.open();
			
			for(Connection connection : connectionsList) {
				connection.initConnection(selector, connection.getPortNumber());
				channelConnectionsMap.put(connection.getSocket(), connection);
			}
			
			while (true) {
				if (0 == selector.select(TIME_TO_WAIT)) {
					System.out.println("channels are not ready");
					continue;
				};
				
		        Set<SelectionKey> selectedKeys = selector.selectedKeys();
		        Iterator<SelectionKey> iter = selectedKeys.iterator();

		        while (iter.hasNext()) {
		        	SelectionKey key = iter.next();
		        	Channel channel = key.channel();
		        	
		        	if (key.isAcceptable()) {
		        		registerTcpClient(selector, (ServerSocketChannel)channel);
		            }

		        	if (key.isReadable()) {
		        		channelConnectionsMap.get(channel).communicateWithClient(channel, buffer);
		        	}
		        	
		        	iter.remove();
		        }  	
		   }
		}

		private void stopConnections() throws IOException {
			Iterator<SelectionKey> keys = selector.keys().iterator();
			
			while(keys.hasNext()) {
				Channel channelToClose = keys.next().channel();
				channelToClose.close();
			}
			
			selector.close();	
		}

		private void addConnection(Connection connection) {
			connectionsList.add(connection);
		}
		
		private void registerTcpClient(Selector selector, ServerSocketChannel tcpSocket) throws IOException {
			SocketChannel client = tcpSocket.accept();
			channelConnectionsMap.put(client, channelConnectionsMap.get(tcpSocket));
			client.configureBlocking(false);
			client.register(selector, SelectionKey.OP_READ);
		}
	}
/**************************** Protocol************************************************/

	private interface Protocol {
		public void handleMessage(ClientInfo clientInfo, Message<?, ?> message, ByteBuffer buffer) 
				throws IOException, ClassNotFoundException;
	}

/************************ PingPong Protocol***************************************/
	private class PingPongProtocol implements Protocol {
		private static final String PONG = "pong";
		private static final String PING = "ping";
		
		@Override
		public void handleMessage(ClientInfo clientInfo, Message<?, ?> message, ByteBuffer buffer)
			throws IOException, ClassNotFoundException {
			PingPongMessage messageToSend = new PingPongMessage(0,getMessageToSend(((InnerMessage)message).getKey()));
			buffer.clear();
			buffer.put(PingPongMessage.toByteArray(messageToSend));
			buffer.flip();
			(connectionHandler.channelConnectionsMap.get(clientInfo.clientChannel)).sendMessage(clientInfo, buffer);
		}
		
		private String getMessageToSend(String input) {
			if(input.equals(PING)) {
	   	    	return PONG;
	   	    }
	   	    else if (input.equals(PONG)) {
	   	    	return PING;
	   	    }
			
	   	    return "wrong data";
		}
	}

/*********************** Message Handler************************************************************/
	private class MessageHandler {
		private Map<Integer, Protocol> protocolsMap = new HashMap<>();
		private int keyCounter = 1;
		
		public MessageHandler() {
			addProtocol(new PingPongProtocol());
		}
		
		public void handleMessage(ByteBuffer messageBuffer, ClientInfo clientInfo) 
			throws ClassNotFoundException, IOException {
			@SuppressWarnings("unchecked")
			Message<Integer, Message<?, ?>> messageObj =  (Message<Integer, Message<?, ?>>) PingPongMessage.
																							toObject(messageBuffer.array());
		//	System.out.println("server recived : " + messageObj);
			Message<?, ?> innerMessage = (Message<?, ?>) messageObj.getData();
			(protocolsMap.get(messageObj.getKey())).handleMessage(clientInfo, innerMessage, messageBuffer) ;
		}

		private void addProtocol(Protocol protocol) {
			protocolsMap.put(keyCounter, protocol);
			++keyCounter;
		}
	}
/************************************ClientInfo***************************************************/
	private class ClientInfo {
		private Channel clientChannel;
		private SocketAddress udpClientAddress;
		
		public ClientInfo(Channel clientChannel) {
			this.clientChannel = clientChannel;
		}
		
		public ClientInfo(Channel clientChannel, SocketAddress clientAddress) {
			udpClientAddress = clientAddress;
			this.clientChannel = clientChannel;
		}	
	}
/***************************************InputDetector*********************************************/    
	private class InputDetector implements Runnable {
		@Override
		public void run() {
			BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
			String input;
			
			try {
				input = reader.readLine();
				while(!input.equals(EXIT)) {
					input = reader.readLine();
				}
				
				stopServer();
				
			} catch (IOException e) {
				e.printStackTrace();
			}
		}	
	} 
/***************************************Exception*********************************************/ 	
	private class AddConnectionException extends RuntimeException {
		private static final long serialVersionUID = 1L;
	}
}
