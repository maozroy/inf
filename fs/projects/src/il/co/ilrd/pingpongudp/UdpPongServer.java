package il.co.ilrd.pingpongudp;

import java.net.DatagramPacket;
import java.net.DatagramSocket;


public class UdpPongServer{

	
    public static void main(String[] args) throws Exception {
		int portNumber = 4444;

		try (DatagramSocket socket = new DatagramSocket(portNumber)){
			for(int i = 0; i <5; ++i) {
				byte[] buf = new byte[256];
				DatagramPacket packet = new DatagramPacket(buf, buf.length);
				socket.receive(packet);
				Thread.sleep(500);
				String received = new String(packet.getData());
				System.out.println("			" + i + "server received = " + received);
				buf = "pong".getBytes();
				packet.setData(buf);
				socket.send(packet);
			}
		} catch (Exception e) {
			// TODO: handle exception
		}
    }
    

}

