package il.co.ilrd.pingpongtcp;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class test {

	public static void main(String[] args) {
		test pingPong = new test();
		pingPong.UDPPingPong();				
	}

	private void UDPPingPong() {
	    Runnable serverrRunnable = new Runnable() {
			public void run() {
				serverMethod();
			}
		};
		new Thread(serverrRunnable).start();
		
		

		
		
		Runnable clientRunnable = new Runnable() {
			public void run() {
				clientMethod();	
			}
		};
		new Thread(clientRunnable).start();
	}
	
	private void serverMethod() {
		try ( 
				DatagramSocket serverSocket = new DatagramSocket(5557);
			) {
				while(true) {
					byte[] buf = new byte[10];
					DatagramPacket packet = new DatagramPacket(buf, buf.length);
					serverSocket.receive(packet);
					String received = new String(packet.getData(), 0, 
															packet.getLength());
					System.out.println(received);
					
					//send response
					buf = "Pong".getBytes();
	                InetAddress address = packet.getAddress();
	                int port = packet.getPort();
	                packet = new DatagramPacket(buf, buf.length, address, port);
	                serverSocket.send(packet);
	                Thread.sleep(1000);
				}
		} catch (Exception e) {
			System.out.println("server " + e);
		}
	}
	
	private void clientMethod() {
		try (
				DatagramSocket socket = new DatagramSocket();
			) {
				while(true) {
			        byte[] buf = new byte[10];
			        buf = "Ping".getBytes();
			        InetAddress address = InetAddress.getByName("maoz-Predator-PH315-51");
			        DatagramPacket packet = new DatagramPacket(buf, buf.length, 
			        											address, 5557);
			        socket.send(packet);
			        
			        // receive response
			        packet = new DatagramPacket(buf, buf.length);
			        socket.receive(packet);
			        String received = new String(packet.getData(), 0, 
			        										packet.getLength());
			        System.out.println(received);
					Thread.sleep(1000);
				}
		} catch (Exception e) {
				System.out.println("client " + e);				
		}
	}

}