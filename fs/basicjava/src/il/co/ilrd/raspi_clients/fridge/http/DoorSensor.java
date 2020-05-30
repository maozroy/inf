package il.co.ilrd.raspi_clients.fridge.http;


public class DoorSensor implements Runnable {
	int msgCounter = 0;
	@Override
	public void run() {
		while(FridgeIOT.isRunning) {
			try {
				Thread.sleep(1000);
				msgCounter++;
				if (msgCounter%5 == 0) {
					FridgeIOT.map.put("ERROR " + msgCounter, "door is open!");
				}
				FridgeIOT.map.put("UPDATE " + msgCounter, "door's closed");
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
		
	}

}
