package il.co.ilrd.multithreaded.tcpudppingpong;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

public class BroadcastPongMultiThreaded {
	private static final int DEFAULT_TCP_PORT = 55000; 
	private static final int DEFAULT_UDP_PORT = 24817;
	private static final int DEFAULT_BROADCAST_PORT = 24818;
	private static final int BUFFER_SIZE = 256;
	private static final String DEFAULT_EXIT = "exit";
	private static final String PONG_STRING = "pong";
	
	private boolean IsClosed = false;
	private static BroadcastPongMultiThreaded server = 
			new BroadcastPongMultiThreaded();
	
	private LinkedBlockingQueue<Closeable> socketlist = 
			new LinkedBlockingQueue<Closeable>();
	private List<Thread> threadList = 
			new LinkedList<Thread>();
	
	public static void main(String[] args) {
		new Thread(server.new TcpPongMultiThreadedServer()).start();
		new Thread(server.new UdpPongServer(DEFAULT_UDP_PORT)).start();
		new Thread(server.new UdpPongServer(DEFAULT_BROADCAST_PORT)).start();
		new Thread(server.new CloseProgram()).start();
	}
	
	private class CloseProgram implements Runnable{

		@Override
		public void run() {
			
			try(BufferedReader buffer = new BufferedReader
					(new InputStreamReader(System.in))) {
				String line = buffer.readLine();
				
				while(!line.equals(DEFAULT_EXIT)) {
					line = buffer.readLine();
				}
				closeAll();
			} catch (IOException | InterruptedException e) {e.printStackTrace();} 
		}

		private void closeAll() throws IOException, InterruptedException {
			server.IsClosed = true;
			
			for (Closeable closeable : socketlist) {
				closeable.close();
			}
			for (Thread thread : threadList) {
				thread.join();
			}
		}
		
	}
	
	private class TcpPongMultiThreadedServer implements Runnable{
		
		@Override
		public void run() {
			try(ServerSocket serverSocket = new ServerSocket(DEFAULT_TCP_PORT)){
				socketlist.add(serverSocket);
				Thread thread;
				Socket socket;
				while(!IsClosed) {
					socket = serverSocket.accept();
					socketlist.add(socket);
					thread = new Thread(new WorkerThreadTCP(socket));
					threadList.add(thread);
					thread.start();
				}
			}catch (SocketException e) {
				if (IsClosed) {
					return;	
				}
			} 
			catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	private class UdpPongServer implements Runnable{
		int port;
		
		public UdpPongServer(int port) {
			this.port = port;
		}

		@Override
		public void run() {
			byte[] buffer;
			DatagramPacket packet;
			String receivedString = null;
			
			try (DatagramSocket socket = new DatagramSocket(port)){
				socketlist.add(socket);
				
				while(!IsClosed) {
					buffer = new byte[BUFFER_SIZE];
					packet = new DatagramPacket(buffer, buffer.length);
					getStringFromSocket(socket, packet, receivedString);
					System.out.println(receivedString);			
					
					setBuffer(packet, buffer, PONG_STRING);
					socket.send(packet);
				}
			}catch (SocketException e) {
				if (IsClosed) {
					return;	
				}
			}
			catch (Exception e) {
				// TODO: handle exception
			}
	    }
	}
	
	private void getStringFromSocket(DatagramSocket socket, DatagramPacket packet, String received) throws IOException {
		socket.receive(packet);
		received = new String(packet.getData());
	}
	
	private void setBuffer(DatagramPacket packet, byte[] buffer, String string) {
		buffer = string.getBytes();
		packet.setData(buffer);		
	}
}
