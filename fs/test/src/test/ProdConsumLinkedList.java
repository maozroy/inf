package test;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.Semaphore;


public class ProdConsumLinkedList {
	static List list = new LinkedList();
	static Runnable prod = new ProduceLinked();
	static Runnable cons = new ConsumeLinked();
	static Semaphore semaphore = new Semaphore(0);

	
	public static void main(String[] args) {
		Thread prodT = new Thread(prod);
		Thread consT = new Thread(cons);
		Thread prodT2 = new Thread(prod);
		Thread consT2 = new Thread(cons);
		Thread prodT3 = new Thread(prod);
		Thread consT3 = new Thread(cons);
		prodT.start();
		consT.start();
		prodT2.start();
		consT2.start();
		prodT3.start();
		consT3.start();
	}

	
}
