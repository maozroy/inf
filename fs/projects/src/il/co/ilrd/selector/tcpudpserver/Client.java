package il.co.ilrd.selector.tcpudpserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SocketChannel;

public class Client {
	private static final int PORT_NUMBER = 50000;
	private static final String hostName = "127.0.0.9";
	
	public static void main(String[] args) throws UnknownHostException, InterruptedException  {
		 InetSocketAddress address = new InetSocketAddress(InetAddress.getLocalHost(), PORT_NUMBER);
		 ByteBuffer buffer = ByteBuffer.allocate(256); 
		 String input = null;
		 SocketChannel client = null;
		 
		try {
			client = SocketChannel.open(address);
		} catch (IOException e1) {
			e1.printStackTrace();
		}  
		
		ExitClient exitThread = new ExitClient(client);
		new Thread(exitThread).start();	
		
		 while(true) {
			 buffer = ByteBuffer.wrap(("ping".getBytes()));
		     try {
				client.write(buffer);
				buffer.clear();
				client.read(buffer);
				
				input = new String(buffer.array()).trim();
				System.out.println("Tcp client received:" + input);
				buffer.clear();
				Thread.sleep(500);
				
				buffer = ByteBuffer.wrap(("pong".getBytes()));
				client.write(buffer);
				buffer.clear();
				client.read(buffer);
				
				input = new String(buffer.array()).trim();
				System.out.println("Tcp client received:" + input);
				buffer.clear();
				Thread.sleep(500);
		    } catch (ClosedChannelException e) {
					return;
			} catch (IOException e) {
				return;
			}
		 }
		 
	}
	
	private static class ExitClient implements Runnable {
		SocketChannel socket;
		
		public ExitClient(SocketChannel socket) {
			this.socket = socket; 
		}
		@Override
		public void run() {
			BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
			String input;
			
			try {
				input = reader.readLine();
				while(!input.equals("exit")) {
					input = reader.readLine();
				}
				
				socket.close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}	
	}
}