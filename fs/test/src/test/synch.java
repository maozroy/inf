package test;

public class synch implements Runnable{
	private static int counter = 0;

	public static void main(String[] args) throws InterruptedException {
		Runnable taskThread = new synch();
		Thread thread = new Thread(taskThread);
		Thread thread2 = new Thread(taskThread);
		long start = System.currentTimeMillis();
		// ...

		thread.start();
		thread2.start();
		thread.join();
		thread2.join();
		long finish = System.currentTimeMillis();
		long timeElapsed = finish - start;
		System.out.println("counter is: " + counter + " time is " + timeElapsed);
	
}

	@Override
	 public void run() {
		int i = 0;
		synchronized (this){
		while (i < 10000000) {
		
			increase();
			
			++i;
		}
		}
		System.out.println(counter);
	}

	private void increase() {
			counter++;
		
	}
}
