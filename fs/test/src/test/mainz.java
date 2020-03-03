package test;

import java.lang.invoke.VolatileCallSite;
import java.util.concurrent.TimeUnit;


public class mainz {
	volatile static Integer counter = 0;

	class Task implements Runnable{
		@Override
		public void run() {
			for (int i = 0; i < 10000000 ; i++) {
				synchronized (counter) {  
					++counter;
				}

			}
			System.out.println("counter is: " + counter);

		}
	}

	
	public static void main(String[] args) throws InterruptedException {
		Task taskThread = new mainz().new Task();
		Thread thread = new Thread(taskThread);
		Thread thread2 = new Thread(taskThread);
		
		thread.start();
		thread2.start();
		thread.join();
		thread2.join();
	}
}
