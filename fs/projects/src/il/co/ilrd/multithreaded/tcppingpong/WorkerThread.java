package il.co.ilrd.multithreaded.tcppingpong;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class WorkerThread implements Runnable{
	Socket socket;

	public WorkerThread(Socket socket) {
		this.socket = socket;
	}

	@Override
	public void run() {
		while(true) {
		try(PrintWriter output = new PrintWriter(socket.getOutputStream());
				BufferedReader input = new BufferedReader(
										new InputStreamReader(
												socket.getInputStream()))){
			System.out.println(input.readLine() + " worker");
			output.write(" pong\n");
			output.flush();
			}
		
		 catch (IOException e) {
			 break;
			
		}
		}
	}
		//System.out.println("loop ended");
	}

	
	
