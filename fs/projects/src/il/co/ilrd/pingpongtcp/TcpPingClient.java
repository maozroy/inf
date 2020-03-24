package il.co.ilrd.pingpongtcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class TcpPingClient {
	boolean isAlive = true;
	
public static void main(String[] args) {
		try(Socket socket = new Socket(InetAddress.getByName("172.20.20.6").getHostName(), 55000);
			PrintWriter output = new PrintWriter(socket.getOutputStream());
			BufferedReader input = new BufferedReader(
									new InputStreamReader(
									socket.getInputStream()))
			){		
			while (true) {
				output.write("ping\n");
				output.flush();
				System.out.println(input.readLine());
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		
	}
}
