package il.co.ilrd.selector.tcpudpserver;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
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
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

public class TamirSelector {
	private static final int TCP_PORT_NUMBER = 50000;
	private static final int UDP_PORT_NUMBER = 55000;
	private final static int BROADCAST_PORT_NUMBER = 60000;
	private static final String hostName = "127.0.0.9";
	private static final String PONG = "pong";
	private static final String PING = "ping";
	private static final String EXIT = "exit";
	private static final int BUFFER_SIZE = 256;

	private Selector selector;
	private ServerSocketChannel tcpSocket;
	private DatagramChannel udpSocket;
	private DatagramChannel broadcastSocket;
	private List<Closeable> socketList = new LinkedList<>();
	ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);

	public void startServer() {
		ExitServer exitThread = new ExitServer();
		new Thread(exitThread).start();

		try {
			selector = Selector.open();
			initTcpServer();
			initUdpServer(udpSocket, new InetSocketAddress(UDP_PORT_NUMBER));
			initUdpServer(broadcastSocket, new InetSocketAddress(InetAddress.getByName("255.255.255.255"), BROADCAST_PORT_NUMBER));

			while (true) {
				selector.select();
				Set<SelectionKey> selectedKeys = selector.selectedKeys();
				Iterator<SelectionKey> iter = selectedKeys.iterator();

				while (iter.hasNext()) {
					SelectionKey key = iter.next();

					if (key.isAcceptable()) {
						registerTcpClient();
					}

					if (key.isReadable()) {
						Channel channel = key.channel();
						if (broadcastSocket == channel) {
							udpHandler(key, "Broadcast");
						}

						else if (udpSocket == channel) {
							udpHandler(key, "UDP");
						}

						else {
							tcpHandler(key, buffer);
						}
					}
					iter.remove();
				}
			}

		} catch (ClosedSelectorException e) {
			return;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void stopServer() throws IOException {
		System.out.println(">>> Exits <<<");
		selector.close();
		for (Closeable socket : socketList) {
			socket.close();
		}
	}

	private String getMessegeToSend(String input) {
		if (input.equalsIgnoreCase(PING)) {
			return PONG;
		} else if (input.equalsIgnoreCase(PONG)) {
			return PING;
		}

		return "Wrong Data. Try again.";
	}
	/*********************************** TCP **************************************/
	private void initTcpServer() throws IOException {
		tcpSocket = ServerSocketChannel.open();
		tcpSocket.configureBlocking(false);
		tcpSocket.bind(new InetSocketAddress(InetAddress.getLocalHost(), TCP_PORT_NUMBER));
		tcpSocket.register(selector, SelectionKey.OP_ACCEPT);
		socketList.add(tcpSocket);
	}

	public void registerTcpClient() throws IOException {
		SocketChannel tcpClient = tcpSocket.accept();
		socketList.add(tcpClient);
		tcpClient.configureBlocking(false);
		tcpClient.register(selector, SelectionKey.OP_READ);
	}

	private void tcpHandler(SelectionKey key, ByteBuffer buffer) throws IOException {
		SocketChannel client = (SocketChannel) key.channel();
		String input = tcpReadData(client, buffer);

		buffer.put(getMessegeToSend(input).getBytes());
		tcpSendData(client);
	}

	private String tcpReadData(SocketChannel clientChannel, ByteBuffer buffer) throws IOException {
		int bytes = clientChannel.read(buffer);

		if (-1 == bytes) {
			clientChannel.close();
			return null;
		}

		String input = new String(buffer.array()).trim();
		buffer.clear();
		System.out.println("Tcp server received:" + input);

		return input;
	}

	private void tcpSendData(SocketChannel clientChannel) throws IOException {
		buffer.flip();
		clientChannel.write(buffer);
		buffer.clear();
	}

	/*********************************** UDP **************************************/
	private void initUdpServer(DatagramChannel udpSocket, InetSocketAddress address) throws IOException {
		udpSocket = DatagramChannel.open();
		udpSocket.configureBlocking(false);
		udpSocket.socket().bind(address);
		udpSocket.register(selector, SelectionKey.OP_READ);
		socketList.add(udpSocket);
	}

	private void udpHandler(SelectionKey key, String serverType) throws IOException {
		DatagramChannel udpChannel = (DatagramChannel) key.channel();
		SocketAddress clientAddress = udpChannel.receive(buffer);
		String input = udpReadData(serverType);
		udpSendData(clientAddress, input);
	}

	private String udpReadData(String serverType) {
		String input = new String(buffer.array(), 0, buffer.position());
		System.out.println(serverType + " server received:" + input);
		buffer.clear();

		return input;
	}

	private void udpSendData(SocketAddress clientAddress, String input) throws IOException {
		buffer.put(getMessegeToSend(input).getBytes());
		buffer.flip();
		udpSocket.send(buffer, clientAddress);
		buffer.clear();
	}

	/********************************* Exit Server ************************************************/
	private class ExitServer implements Runnable {
		@Override
		public void run() {
			BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
			String input;

			try {
				input = reader.readLine();
				while (!input.equalsIgnoreCase(EXIT)) {
					input = reader.readLine();
				}

				stopServer();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}