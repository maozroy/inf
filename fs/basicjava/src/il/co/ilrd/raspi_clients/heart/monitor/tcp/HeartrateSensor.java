package il.co.ilrd.raspi_clients.heart.monitor.tcp;

public class HeartrateSensor implements Runnable {
	static int msgCounter = 0;


	@Override
	public void run() {
		while(HeartMonitorIOT.isRunning) {
			try {
				Thread.sleep(1000);
				msgCounter++;
				HeartMonitorIOT.map.put("UPDATE " + msgCounter, "heart rate is 100");
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
		// TODO Auto-generated method stub
		
	}
}