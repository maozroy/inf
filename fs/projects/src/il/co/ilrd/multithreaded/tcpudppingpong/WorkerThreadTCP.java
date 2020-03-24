package il.co.ilrd.multithreaded.tcpudppingpong;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class WorkerThreadTCP implements Runnable{
	Socket socket;

	public WorkerThreadTCP(Socket socket) {
		this.socket = socket;
	}

	@Override
	public void run() {
		String line;
		try(PrintWriter output = new PrintWriter(socket.getOutputStream());
				BufferedReader input = new BufferedReader(
										new InputStreamReader(
												socket.getInputStream()))){
			while(true) {
				System.out.println("s");
				line = input.readLine();
				if (null == line ) {
					break;
				}
				System.out.println(line);
				output.write(" pong\n");
				output.flush();
			}
		}
		catch (IOException e) {
		 System.out.println("exepction " + e);
		}
	}
}

	
	
