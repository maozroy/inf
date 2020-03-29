package il.co.ilrd.selector.tcpudpserver;

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
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Set;

public class SelectorServer{
	private static final int DEFAULT_TCP_PORT = 50000; 
	private static final int DEFAULT_UDP_PORT = 24817;
	private static final int DEFAULT_BROADCAST_PORT = 24818;
	private static final int DEFAULT_BUFFER_SIZE = 256;
	private static final String EXIT_STRING = "exit";
	
	private int tcpPort;
	private int udpPort;
	private int bCPort;
	private InetAddress addrs;
	
	private MainServer server;
	private KillServer killService;

	//----CTORS----//                                             
	public SelectorServer(int tcpPort, int udpPort, int bCPort, InetAddress addrs) {
		this.tcpPort = tcpPort;
		this.udpPort = udpPort;
		this.bCPort = bCPort;
		this.addrs = addrs;
		server = new MainServer();
		killService = new KillServer(EXIT_STRING);
	}
	
	public SelectorServer(InetAddress ip) throws UnknownHostException {
		this(DEFAULT_TCP_PORT, DEFAULT_UDP_PORT, DEFAULT_BROADCAST_PORT, ip);
	}
	
	public SelectorServer() throws UnknownHostException {
		this(DEFAULT_TCP_PORT, DEFAULT_UDP_PORT, DEFAULT_BROADCAST_PORT, InetAddress.getLocalHost());
	}
	
	public void RunServer() throws IOException {
		new Thread(killService).start();
		server.RunServer();
	}

	//----KillService----//
	private class KillServer implements Runnable{
		String killString;
		
		public KillServer(String killString) {
			this.killString = killString;
		}
		
		@Override
		public void run() {
			try(BufferedReader buffer = new BufferedReader
					(new InputStreamReader(System.in))) {
				String line = buffer.readLine();
				
				while(!line.equals(killString)) {
					line = buffer.readLine();
				}
				closeAll();
			} catch (IOException e) {e.printStackTrace();} 			
		}

		private void closeAll() throws IOException {
			server.selector.close();			
			for (Closeable socket : server.socketList) {
				socket.close();
			}
		}
	}

	//----Server---//
	private class MainServer{
		private static final String PONG = "pong";
		private static final String PING = "ping";

	    private Selector selector;
		private ServerSocketChannel TCPChannel;
		private DatagramChannel UDPChannel; 
		private DatagramChannel BCChannel;
		private LinkedList<Closeable> socketList = new LinkedList<>();
		ByteBuffer buffer = ByteBuffer.allocate(DEFAULT_BUFFER_SIZE);

		public void RunServer() 
				throws IOException {

	        try {
	        	selector = Selector.open();
	        	initSelectorTCP();
	        	initSelectorUDP(UDPChannel, udpPort, addrs);
	        	initSelectorUDP(BCChannel, bCPort,InetAddress.getByName("255.255.255.255"));
	        	
		        while (true) {
		            selector.select();
		            Set<SelectionKey> selectedKeys = selector.selectedKeys();
		            Iterator<SelectionKey> iter = selectedKeys.iterator();
		            
		            while (iter.hasNext()) {
		                 SelectionKey key = iter.next();
		               
			            if (key.isAcceptable()) {
			            	 registerTCPClient();
			            }
			            else if (key.isReadable()) {
		                	ReadKey(key);
		                }
		                iter.remove();
		            }
		        }
	        }
	        catch (ClosedSelectorException e) {
				return;
	        }
		}
	
		private void ReadKey(SelectionKey key) throws IOException {
        	Channel channel = key.channel();
        	
        	if (channel == TCPChannel) {
        		TCPHandler(key, buffer);
			} else {
				UDPHandler(key,buffer);
			}			
		}

		private void initSelectorUDP(DatagramChannel channel, int port, InetAddress addrs) 
				throws IOException {
			channel = DatagramChannel.open();
			channel.configureBlocking(false);
			channel.socket().bind(new InetSocketAddress(addrs,port));
			channel.register(selector, SelectionKey.OP_READ);
			socketList.add(channel);
		}
		
		private void initSelectorTCP() 
				throws IOException {
			TCPChannel = ServerSocketChannel.open();
			TCPChannel.configureBlocking(false);
			TCPChannel.bind(new InetSocketAddress(addrs, tcpPort));
			TCPChannel.register(selector, SelectionKey.OP_ACCEPT);   
	        socketList.add(TCPChannel);		
		}
		
		private void registerTCPClient() 
				throws IOException{
			SocketChannel client = TCPChannel.accept();
			socketList.add(client);
			client.configureBlocking(false);
			client.register(selector, SelectionKey.OP_READ);
		}
		
		private void TCPHandler(SelectionKey key, ByteBuffer buffer) 
				throws IOException {
			try {
				SocketChannel client = (SocketChannel) key.channel();
				String input = TCPBufferToString(client, buffer);
				buffer.put(pingOrPong(input).getBytes());
				System.out.println(input);
				TCPSendData(buffer,client);
			} catch (NullPointerException e) {
				System.out.println("Client ended communication");
				return;
			}
		}
		
		private void UDPHandler(SelectionKey key, 
				ByteBuffer buffer) throws IOException {
			@SuppressWarnings("unused")
			DatagramChannel udpChannel = (DatagramChannel) key.channel();
			SocketAddress clientAddress = udpChannel.receive(buffer);
			String input = UDPbufferToString(buffer);
			System.out.println(input);
			UDPSendData(buffer, udpChannel, clientAddress,input);	
		}
		
		private void TCPSendData(ByteBuffer buffer, SocketChannel client)
				throws IOException{
			buffer.flip();
			client.write(buffer);
			buffer.clear();
		}
	
		private void UDPSendData(ByteBuffer buffer, 
			DatagramChannel udpChannel, 
			SocketAddress clientAddress, 
			String input) throws IOException {
			
			buffer.put(pingOrPong(input).getBytes());
			buffer.flip();
			udpChannel.send(buffer, clientAddress);
			buffer.clear();
		}
	
	
		private String TCPBufferToString(SocketChannel client, 
										ByteBuffer buffer) 
										throws IOException {
			int bytes = client.read(buffer);
			
			if (-1 == bytes) {
				client.close();
				return null;
			}
			String input = new String(buffer.array()).trim();
			buffer.clear();
			
			return input;
		}
	
		private String UDPbufferToString(ByteBuffer buffer) {
			return new String(buffer.array(), 0,buffer.position());
		}
		
		private String pingOrPong(String input) {
			
			if (input.equalsIgnoreCase(PING)) {
				return PONG;
			} else if (input.equalsIgnoreCase(PONG)) {
				return PING;
			}
			return "I'm not playing with you";
		}
	}
}