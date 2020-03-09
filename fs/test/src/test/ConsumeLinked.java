package test;

import java.util.List;
import java.util.concurrent.Semaphore;

public class ConsumeLinked implements Runnable{

	@Override
	public void run() {
		
		while(true) {
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			synchpop(ProdConsumLinkedList.list,ProdConsumLinkedList.semaphore);
			
			
			
			
		}		
	}

	private void synchpop(List list, Semaphore semaphore) {
		synchronized(ProdConsumLinkedList.list) {

				if (semaphore.tryAcquire()) {
					System.out.println("consume id: " + Thread.currentThread().getName() + "item "+ ProdConsumLinkedList.list.remove(0));				
				};
				

			}
		}		
	}


