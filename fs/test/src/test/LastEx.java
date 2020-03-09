package test;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;


public class LastEx {
	static int numOfCons = 5;
	static Semaphore semaphore = new Semaphore(0);
	static Integer globalDataInteger = 0;
	static ReentrantLock lock = new ReentrantLock();
	static Condition condition = lock.newCondition();
	
	public static void main(String[] args) {
		Thread prodThread = new Thread(new LastProd());
		Thread consThread1 = new Thread(new LastCons());
		Thread consThread2 = new Thread(new LastCons());
		Thread consThread3 = new Thread(new LastCons());
		Thread consThread4 = new Thread(new LastCons());
		numOfCons = 4;
		
		prodThread.start();
		consThread1.start();
		consThread2.start();
		consThread3.start();
		consThread4.start();

	}
}
