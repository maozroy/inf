package il.co.ilrd.pingpong.handlers;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SocketChannel;

public class TCPClient {
	private final static int PORT_NUM_TCP = 60000;

	public static void main(String[] args) throws IOException, ClassNotFoundException {
		ServerMessage message1 = new ServerMessage(ProtocolIndex.PINGPONG, new PingPongMessage("pong"));
		byte[] array1 = PingPongMessage.toByteArray(message1);
		ServerMessage message2 = new ServerMessage(ProtocolIndex.PINGPONG, new PingPongMessage("ping"));
		byte[] array2 = PingPongMessage.toByteArray(message2);
		InetSocketAddress address = new InetSocketAddress(InetAddress.getLocalHost(), PORT_NUM_TCP);
		ByteBuffer buffer = ByteBuffer.allocate(1000); 
		SocketChannel client = null;
		
		try {
			client = SocketChannel.open(address);
		} catch (IOException e1) {
			e1.printStackTrace();
		}  
		
		ExitClient exitThread = new ExitClient(client);
		new Thread(exitThread).start();	
		
		while(true) {
			array1 = PingPongMessage.toByteArray(message1);
			array2 = PingPongMessage.toByteArray(message2);
			 buffer = ByteBuffer.wrap(array1);
		     try {
				client.write(buffer);
				System.out.println("Tcp client send: " + PingPongMessage.toObject(buffer.array()));
				buffer.clear();
				System.out.println("before read");
				client.read(buffer);
				System.out.println("after read");

				System.out.println("Tcp client received: " + PingPongMessage.toObject(buffer.array()));
				System.out.println("after print");
				buffer.clear();
				Thread.sleep(500);
				buffer = ByteBuffer.wrap(array2);
				client.write(buffer);
				System.out.println("Tcp client send: " + PingPongMessage.toObject(buffer.array()));
				buffer.clear();
				client.read(buffer);
				System.out.println("after read");
				System.out.println("Tcp client received: " + PingPongMessage.toObject(buffer.array()));
				buffer.clear();
				Thread.sleep(500);
		    } catch (ClosedChannelException e) {
					return;
			} catch (IOException e) {
				return;
			} catch (InterruptedException e) {
				e.printStackTrace();
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