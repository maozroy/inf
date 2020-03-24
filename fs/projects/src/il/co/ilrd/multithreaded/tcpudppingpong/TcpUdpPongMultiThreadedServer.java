package il.co.ilrd.multithreaded.tcpudppingpong;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.ServerSocket;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;





public class TcpUdpPongMultiThreadedServer {
	static final int DEFAULT_TCP_PORT = 55000; 
	static final int DEFAULT_UDP_PORT = 24817;
	static final int DEFAULT_BROADCAST_PORT = 24818;
	static final String DEFAULT_EXIT = "exit";
	
	boolean IsClosed = false;
	List<Closeable> socketlist = new ArrayList<Closeable>();
	Lock listlock = new ReentrantLock();
	static TcpUdpPongMultiThreadedServer server = new TcpUdpPongMultiThreadedServer();
	
	public static void main(String[] args) {
		new Thread(server.new TcpPongMultiThreadedServer()).start();
		new Thread(server.new UdpPongServer()).start();
		new Thread(server.new BroadcastPongServer()).start();
		new Thread(server.new CloseProgram()).start();
	}
	
	public class CloseProgram implements Runnable{

		@Override
		public void run() {
			
			try(BufferedReader buffer = new BufferedReader(new InputStreamReader(System.in));) {
				String line = buffer.readLine();
				
				while(!line.equals(DEFAULT_EXIT)) {
					line = buffer.readLine();
				}
				closeSockets();
			} catch (IOException e) {e.printStackTrace();} 
		}

		private void closeSockets() throws IOException {
			server.IsClosed = true;
			for (Closeable closeable : socketlist) {
				closeable.close();
			}
		}
		
	}
	
	public class BroadcastPongServer implements Runnable{

		@Override
		public void run() {
			try (DatagramSocket socket = new DatagramSocket(DEFAULT_BROADCAST_PORT)){
				safeAdd(socket);
					
				while(!IsClosed) {
					byte[] buffer = new byte[256];
					DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
					
					socket.receive(packet);
					String received = new String(packet.getData());
					System.out.println(received);
					
					buffer = "pong".getBytes();
					packet.setData(buffer);
					socket.send(packet);
				}
				
			}catch (SocketException e) {
				if (IsClosed) {
					return;	
				}
			} 
			catch (Exception e) {}
	    }
	    

	}

	public class TcpPongMultiThreadedServer implements Runnable{
		
		@Override
		public void run() {
			try(ServerSocket serverSocket = new ServerSocket(DEFAULT_TCP_PORT);){
				safeAdd(serverSocket);
				while(!IsClosed) {	
					new Thread(new WorkerThreadTCP(serverSocket.accept())).start();
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
	
	public class UdpPongServer implements Runnable{

		
		@Override
		public void run() {
			try (DatagramSocket socket = new DatagramSocket(DEFAULT_UDP_PORT)){
				safeAdd(socket);
				
				while(!IsClosed) {
					byte[] buffer = new byte[256];
					DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
					
					socket.receive(packet);
					String received = new String(packet.getData());
					System.out.println(received);
					setBuffer(packet, buffer, "pong");
					buffer = "pong".getBytes();
					packet.setData(buffer);
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
	private void setBuffer(DatagramPacket packet, byte[] buffer, String string) {
		buffer = string.getBytes();
		packet.setData(buffer);		
	}
	
	private void safeAdd(Closeable socket) {
		listlock.lock();
		socketlist.add(socket);
		listlock.unlock();		
	}
}
