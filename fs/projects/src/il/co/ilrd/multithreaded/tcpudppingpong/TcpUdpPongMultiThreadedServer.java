package il.co.ilrd.multithreaded.tcpudppingpong;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.ServerSocket;
import java.net.Socket;




public class TcpUdpPongMultiThreadedServer {
	static final int DEFAULT_TCP_PORT = 24816; 
	static final int DEFAULT_UDP_PORT = 24817;
	public static void main(String[] args) {
		TcpUdpPongMultiThreadedServer server = new TcpUdpPongMultiThreadedServer();
		new Thread(server.new TcpPongMultiThreadedServer()).start();
		new Thread(server.new UdpPongServer()).start();

	}

	
	public class TcpPongMultiThreadedServer implements Runnable{
		TcpPongMultiThreadedServer server =  this;
		
		@Override
		public void run() {
			System.out.println("tcp server out");
			try(ServerSocket serverSocket = new ServerSocket(DEFAULT_TCP_PORT);
				){
				while(true) {	
					new Thread(new WorkerThreadTCP(serverSocket.accept())).start();
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			}
		}
	
	public class UdpPongServer implements Runnable{

		
		@Override
		public void run() {
			System.out.println("udp server start");
			try (DatagramSocket socket = new DatagramSocket(DEFAULT_UDP_PORT)){
				byte[] buf = new byte[256];
				DatagramPacket packet = new DatagramPacket(buf, buf.length);
				
				while(true) {
					socket.receive(packet);
					new Thread(new WorkerThreadUDP(socket, packet)).start();
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
	    }
	    

	}
}
