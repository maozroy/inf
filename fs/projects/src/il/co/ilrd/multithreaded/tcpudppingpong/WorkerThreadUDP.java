package il.co.ilrd.multithreaded.tcpudppingpong;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Socket;

public class WorkerThreadUDP implements Runnable{
	DatagramSocket socket;
	DatagramPacket packet;

	public WorkerThreadUDP(DatagramSocket socket, DatagramPacket packet) {
		this.socket = socket;
		this.packet = packet;

	}

	@Override
	public void run() {
		byte[] buf = new byte[256];
		int counter = 0;
		System.out.println("worker udp started");
		while(!socket.isClosed()) {
			++counter;
			String received = new String(packet.getData());
			System.out.println(received + counter);
			buf = "pong".getBytes();
			packet.setData(buf);
			
			try {
				socket.send(packet);
			} catch (IOException e) {
				System.err.println("sending failed");
				break;
			}
		}
		}
	}

	
	
