package il.co.ilrd.multithreaded.tcppingpong;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class TcpPongMultiThreadedServer extends Thread{
	static TcpPongMultiThreadedServer server =  new TcpPongMultiThreadedServer();
	static final int DEFAULT_PORT = 24816; 
	
	public static void main(String[] args) {
		int port = DEFAULT_PORT;
		

		try(ServerSocket serverSocket = new ServerSocket(port);
			){
			while(true) {	
				Socket socket = serverSocket.accept();
				new Thread(new WorkerThread(socket)).start();
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		}
	}

		

	
	

