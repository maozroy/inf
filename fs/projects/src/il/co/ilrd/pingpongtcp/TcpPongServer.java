package il.co.ilrd.pingpongtcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;



public class TcpPongServer implements Runnable{
	boolean isAlive = true;
	
	@Override
	public void run() {
		try(ServerSocket serverSocket = new ServerSocket(24816);
			Socket socket = serverSocket.accept();
			PrintWriter output = new PrintWriter(socket.getOutputStream());
			BufferedReader input = new BufferedReader(
									new InputStreamReader(
									socket.getInputStream()))
			){	
			while (isAlive) {
			//	Thread.sleep(500);
				System.out.println(input.readLine());
				output.write(" pong\n");
				output.flush();
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		
	}
}
