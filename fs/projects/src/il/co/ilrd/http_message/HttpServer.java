package il.co.ilrd.http_message;

import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;


public class HttpServer implements Runnable{
	private boolean isRunning = false;
	private ByteBuffer messageBuffer;
	private Selector selector;
	private static int BUFFER_CAP = 4096;
	private int port = 8080; 
	@Override
	public void run() {
		isRunning = true;
		messageBuffer = ByteBuffer.allocate(BUFFER_CAP);
		try {
			selector = Selector.open();
			ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
			serverSocketChannel .configureBlocking(false);
			serverSocketChannel .bind(new InetSocketAddress(port));
			serverSocketChannel .register(selector, SelectionKey.OP_ACCEPT);
			selector.select();
			SocketChannel socketChannel = serverSocketChannel.accept();
			socketChannel.read(messageBuffer);
	//		System.out.println(new String(messageBuffer.array()));
			String inString = new String(messageBuffer.array());
			HttpParser httpParser = new HttpParser(inString);
			System.out.println(httpParser.getBody());
			System.out.println(httpParser.getStartLine().getHttpMethod());
			System.out.println(httpParser.getStartLine().getHttpVersion());
			System.out.println(httpParser.getStartLine().getURL());
			for (String string : httpParser.getHeader().getAllHeaders().keySet()) {
				System.out.println(string);
				System.out.println(httpParser.getHeader().getAllHeaders().get(string));
			}

			messageBuffer.clear();
			messageBuffer.put("HTTP/1.1 200 ok".getBytes());
			messageBuffer.flip();
			socketChannel.write(messageBuffer);
		//	System.out.println(socketChannel.getRemoteAddress());
			//
//			while (true) {
//				if (0 == selector.select(5000)) {
//					System.out.println("server running");
//					continue;
//				}
//				Set<SelectionKey> selectedKeys = selector.selectedKeys();
//				Iterator<SelectionKey> iter = selectedKeys.iterator();
//				while (iter.hasNext()) {
//					SelectionKey key = iter.next();
//					if (key.isAcceptable() && key.isValid())  {
//						System.out.println("accept");
//						Thread.sleep(10000);
//					}
//					if (key.isReadable() && key.isValid()) {
//						Channel client = key.channel();
//						System.out.println("read");
//					}
//				}
//				iter.remove();
//			}
//
		} catch (Exception e) {
			if (isRunning) {
				throw new RuntimeException(e);
			}
		}
	}
	
	private  void printStringParts(String[] strings) {
		System.out.println("printing parts");
		for (int i = 0; i < strings.length; i++) {
//			if (strings[i].equals("\r"))
//				System.out.println("line");
			System.out.println(strings[i]);
		}
		System.out.println("size " + strings.length);
	}
}
