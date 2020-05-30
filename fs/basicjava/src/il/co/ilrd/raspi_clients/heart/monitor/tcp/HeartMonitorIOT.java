package il.co.ilrd.raspi_clients.heart.monitor.tcp;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SocketChannel;
import java.util.LinkedList;
import java.util.concurrent.ConcurrentHashMap;


public class HeartMonitorIOT  {
	protected static final String DB_NAME = "tadiran";
	protected static final String DB_NAME_FIELD = "dbName";
	protected static final String KEY_COMMAND_FIELD = "CommandKey";
	protected static final String IOT_UPDATE = "IOT_UPDATE";
	protected static final String RAW_DATA = "rawData";
	protected static final String APOSTROPHE = "'";
	protected static final String TIME_DATE_PATTERN = "yyyy-MM-dd HH:mm:ss";
	protected static final String DELIMETER = "|";
	protected static final String DATA = "Data";
	
	protected static final int serialNumber = 0;
	protected static final String IP = "127.0.0.1";
	protected static final int PORT = 50000;
	protected static final int BUFFER_SIZE = 4096;
	protected static SocketChannel clientSocket = null;
	private static RecieveMessage recieveMsesage = new RecieveMessage();
	private static SendMessage sendMsesage = new SendMessage();
	private static HeartrateSensor heartrateSensor = new HeartrateSensor();
	protected static ConcurrentHashMap<String, String> map = new ConcurrentHashMap<String, String>();

	public static boolean isRunning = true;
	private static LinkedList<Thread> threads = new LinkedList<Thread>();
	public static void main(String[] args) {
		threads.add(new Thread(recieveMsesage));
		threads.add(new Thread(sendMsesage));
		threads.add(new Thread(heartrateSensor));
		try {
			initConnection();
		} catch (IOException e) {
			System.err.println("Connecting to server failed");
			e.printStackTrace();
			return;
		}
		
	}

	private static void initConnection() throws IOException {
		clientSocket = SocketChannel.open(new InetSocketAddress(IP, PORT));	
		for (Thread thread : threads) {
			thread.start();
		}
		System.out.println("Connecting heartMonitor to " + IP + " in port " + PORT);
		
	}

	public static void close() {
		isRunning = false;		
	}
}
