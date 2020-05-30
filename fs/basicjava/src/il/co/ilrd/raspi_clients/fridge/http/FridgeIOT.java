package il.co.ilrd.raspi_clients.fridge.http;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.LinkedList;
import java.util.concurrent.ConcurrentHashMap;

public class FridgeIOT {
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
	protected static final int PORT = 8080;
	protected static final int BUFFER_SIZE = 4096;
	protected static Socket socket;
	private static RecieveMessage recieveMsesage = new RecieveMessage();
	private static SendMessage sendMsesage = new SendMessage();
	private static DoorSensor doorSensor = new DoorSensor();
	protected static ConcurrentHashMap<String, String> map = new ConcurrentHashMap<String, String>();

	public static boolean isRunning = true;
	private static LinkedList<Thread> threads = new LinkedList<Thread>();
	public static void main(String[] args) {
		threads.add(new Thread(recieveMsesage));
		threads.add(new Thread(sendMsesage));
		threads.add(new Thread(doorSensor));
		try {
			initConnection();
		} catch (IOException e) {
			System.err.println("Connecting to server failed");
			e.printStackTrace();
			return;
		}
	}

	private static void initConnection() throws IOException {
		socket = new Socket(InetAddress.getByName(IP), PORT);
		for (Thread thread : threads) {
			thread.start();
		}
		System.out.println("Connecting fridgeIOT to " + IP + " in port " + PORT);
	}

	public static void close() {
		isRunning = false;		
	}
}
