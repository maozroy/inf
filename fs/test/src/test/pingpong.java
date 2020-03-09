package test;

import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class pingpong {
	static Semaphore pingS = new Semaphore(0);
	static Semaphore pongS = new Semaphore(1);

	
	public static void main(String[] args) {

		Thread thread = new Thread(new ping());
		Thread thread2 = new Thread(new pong());
		thread.start();
		thread2.start();

		
	}
}
