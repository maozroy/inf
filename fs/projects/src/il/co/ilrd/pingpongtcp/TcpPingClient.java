package il.co.ilrd.pingpongtcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

public class TcpPingClient {
	boolean isAlive = true;
	
public static void main(String[] args) throws InterruptedException {
		try(Socket socket = new Socket(InetAddress.getLocalHost(), 55000);
			PrintWriter output = new PrintWriter(socket.getOutputStream());
			BufferedReader input = new BufferedReader(
									new InputStreamReader(
									socket.getInputStream()))
			){		
			for (int i = 0 ; i < 5 ;i++) {
			//	Thread.sleep(2500);
				output.write("ping\n");
				output.flush();
				System.out.println("server sends: " + input.readLine());
			}
		socket.close();	
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		
	}
}
