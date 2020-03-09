package test;

import java.util.concurrent.atomic.AtomicInteger;

public class mainz implements Runnable{
	volatile private static Integer counter = 0;
		static AtomicInteger atom = new AtomicInteger(0);

		@Override
		public void run() {
			//	synchronized (this) {
			System.out.println(Thread.currentThread());

					for (int i = 0; i < 10000000 ; i++) {
						atom.incrementAndGet();
					}

			//	}	


			System.out.println("counter is: " + atom.get());
		}
	

	
	public static void main(String[] args) throws InterruptedException {
		Runnable taskThread = new mainz();
		Thread thread = new Thread(taskThread);
		Thread thread2 = new Thread(taskThread);
		
		thread.start();
		thread2.start();
		thread.join();
		thread2.join();
		System.out.println("counter is: " + mainz.atom.get());
	}
}
