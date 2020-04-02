package il.co.ilrd.pingpong.handlers;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;


public class UDPClient {

public class UdpMessageClient {
	private boolean isRunning = true;
	private long id = Thread.currentThread().getId();

	public void startClient() throws IOException, IOException, InterruptedException, ClassNotFoundException {
		int portNumber = 5000;

		System.out.println("Connecting udp to " + InetAddress.getLocalHost() + " in port " + portNumber);

		byte[] reqBuf = new byte[256];
		DatagramPacket packet = new DatagramPacket(reqBuf, reqBuf.length, InetAddress.getByName("255.255.255.255"), portNumber);

		try (DatagramSocket datagramSocket = new DatagramSocket();) {
//			InputDetector exitDetector = new InputDetector(() -> stopUdpClient(datagramSocket), "exit");
//			exitDetector.start();
			while (isRunning) {
				Thread.sleep(1000);
				sendData(datagramSocket, packet);
				datagramSocket.receive(packet);
				printReceivedData(packet);
			}
		} catch (SocketException e) {
			System.err.println("closing udp client");

		}
	}

	private void sendData(DatagramSocket datagramSocket, DatagramPacket packet) throws IOException {
		Message<Integer, Message<String, Void>> message = new PingPongMessage(1, "pong");
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		ObjectOutputStream out = new ObjectOutputStream(bos);
		out.writeObject(message);
		out.flush();
		byte[] reqBuf = bos.toByteArray();
//		byte[] reqBuf = new byte[256];
		// reqBuf = ("udp pong " + id).getBytes();
		packet.setData(reqBuf);
		datagramSocket.send(packet);
	}

	private void stopUdpClient(DatagramSocket datagramSocket) {
		isRunning = false;
		datagramSocket.close();
	}

	private void printReceivedData(DatagramPacket packet) throws ClassNotFoundException, IOException {
        byte[] data = packet.getData();
        PingPongMessage message = (PingPongMessage) PingPongMessage.toObject(data);
//        Message<Integer, Message<String, Void>> message = null;
//        ByteArrayInputStream in = new ByteArrayInputStream(data);
//    	try {
//            ObjectInputStream is = new ObjectInputStream(in);
//
//			message = (Message<Integer, Message<String, Void>>) is.readObject();
//		} catch (ClassNotFoundException e) {
//			e.printStackTrace();
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
    	System.out.println(message);
		System.out.println("Client channel UDP " + id + " received: " + message.getData().getKey());
	}
}
	public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {

		new UDPClient().new UdpMessageClient().startClient();
//		PingPongMessage msg = new PingPongMessage(1, "ping");
//		int portNumber = 50000;
//		DatagramPacket packet = null;
//		DatagramPacket recievepacket = null;
//		try (DatagramSocket socket = new DatagramSocket()){
//		for(int i = 0; i < 5 ;i++){
//			
//			
//			
//			
//				System.out.println("starting loop: " + msg.getData());
//				byte[] arr = PingPongMessage.toByteArray(msg);
//				
//				packet = new DatagramPacket(arr, arr.length, InetAddress.getLocalHost(), portNumber);
//				socket.send(packet);
//				
//				recievepacket = new DatagramPacket(arr, arr.length, InetAddress.getLocalHost(), portNumber);
//				socket.receive(recievepacket);
//				
//				Thread.sleep(500);
//				msg = (PingPongMessage) PingPongMessage.toObject(recievepacket.getData());
//				System.out.println("returned msg: "+msg.getData());
//			}
//		} catch (Exception e) {}
//	}
	}
}


