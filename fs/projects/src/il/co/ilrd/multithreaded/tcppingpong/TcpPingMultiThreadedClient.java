package il.co.ilrd.multithreaded.tcppingpong;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class TcpPingMultiThreadedClient {
	boolean isAlive = true;
	public static void main(String[] args) throws InterruptedException {
	
			Thread.sleep(1000);
		try(Socket socket = new Socket("maoz-Predator-PH315-51", 24816);
		PrintWriter output = new PrintWriter(socket.getOutputStream());
		BufferedReader input = new BufferedReader(
								new InputStreamReader(
								socket.getInputStream()))
		){		
			for(int i = 0 ; i < 5 ; i++) {
			output.write("ping\n");
			output.flush();
			System.out.println(input.readLine());
		//	Thread.sleep(500);
			}
		}
		 catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		}
	}


