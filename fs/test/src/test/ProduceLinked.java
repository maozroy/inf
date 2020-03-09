package test;

import java.util.List;
import java.util.concurrent.Semaphore;

public class ProduceLinked implements Runnable{
	static int counter = 0;
	@Override
	public void run() {
		while(true) {
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			synchadd(ProdConsumLinkedList.list,ProdConsumLinkedList.semaphore);
		}
	}
	private void synchadd(List list, Semaphore semaphore) {
		synchronized(ProdConsumLinkedList.list) {
				ProdConsumLinkedList.list.add(++counter);
				System.out.println("produce id: " + Thread.currentThread().getName());
				semaphore.release();
		}		
	}

}
