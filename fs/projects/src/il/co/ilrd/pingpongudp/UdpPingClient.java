package il.co.ilrd.pingpongudp;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class UdpPingClient {
public static void main(String[] args) throws SocketException {
		int portNumber = 24817;
		DatagramPacket packet = null;
		DatagramPacket recievepacket = null;

		byte[] buf = new byte[256];
		try (DatagramSocket socket = new DatagramSocket()){
			buf = "ping".getBytes();
			for(int i = 0; i <5; ++i) {
				packet = new DatagramPacket(buf, buf.length, InetAddress.getLocalHost(), portNumber);
				socket.send(packet);
				System.out.println("sent");
				recievepacket = new DatagramPacket(buf, buf.length, InetAddress.getLocalHost(), portNumber);
				socket.getSoTimeout();
				socket.receive(recievepacket);
				
				Thread.sleep(500);
				
				String received = new String(recievepacket.getData());
				System.out.println(i + "client received = " + received);
			}
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
}

