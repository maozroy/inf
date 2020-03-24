package il.co.ilrd.pingpongudp;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.List;

public class UdpPingClient {
	static List<Closeable> socketlist = new ArrayList<Closeable>();
	static Thread closingThread = new Thread(new CloseProgram());
	
	static public class CloseProgram implements Runnable{
		@Override
		public void run() {
			BufferedReader buffer = new BufferedReader(new InputStreamReader(System.in));
			try {
				String line = buffer.readLine();
				
				while(!line.equals("exit")) {
					System.err.println(line);
					line = buffer.readLine();
				}
				closeSockets();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
		}

		private void closeSockets() throws IOException {
			for (Closeable closeable : socketlist) {
				closeable.close();
			}
			System.out.println("close thread");
		}
	}
	
	
public static void main(String[] args) throws SocketException {
		int portNumber = 24817;
		DatagramPacket packet = null;
		DatagramPacket recievepacket = null;
		closingThread.start();
		byte[] buf = new byte[256];
		try (DatagramSocket socket = new DatagramSocket()){
			socketlist.add(socket);
			System.out.println(socketlist.size());
			while(true) {
				buf = "ping".getBytes();
				packet = new DatagramPacket(buf, buf.length, InetAddress.getLocalHost(), portNumber);
				socket.send(packet);
				recievepacket = new DatagramPacket(buf, buf.length, InetAddress.getLocalHost(), portNumber);
				socket.getSoTimeout();
				socket.receive(recievepacket);
				
				Thread.sleep(500);
				
				String received = new String(recievepacket.getData());
				System.out.println("client received = " + received);
			}
		} catch (Exception e) {}
	}
}

