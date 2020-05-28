package il.co.ilrd.raspi_clients.heart.monitor.tcp;

import java.net.InetAddress;
import java.util.concurrent.ConcurrentHashMap;

import il.co.ilrd.raspi_clients.IOTDevice;

public class HeartMonitorIOT  {
	private static final String COMPANY_NAME = "";
	
	protected static final int serialNumber = 0;
	protected static final String IP = "127.0.0.1";
	protected static final int PORT = 55555;

	private static RecieveMessage recieveMsesage = new RecieveMessage();
	private static SendMessage sendMsesage = new SendMessage();
	private static HeartrateSensor heartrateSensor = new HeartrateSensor();
	protected static ConcurrentHashMap<String, String> map = new ConcurrentHashMap<String, String>();

	public static boolean isRunning = true;
	
	public static void main(String[] args) {
		new Thread(recieveMsesage).start();
		new Thread(sendMsesage).start();
		new Thread(heartrateSensor).start();
		
		
	}
	

}
