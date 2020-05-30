package testnative;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.ReentrantLock;

public class FixedSizeQueue<T>{
	private ArrayList<T> arrayList;
	private Semaphore readSemaphore = new Semaphore(0);
	private Semaphore writeSemaphore;
	private ReentrantLock readLock = new ReentrantLock();
	private ReentrantLock writeLock = new ReentrantLock();
	
	public FixedSizeQueue(int size){
		arrayList = new ArrayList<T>(size);
		writeSemaphore = new Semaphore(size);
	}
	
	public void writeToQueue(T data) throws InterruptedException {
		writeLock.lock();
		writeSemaphore.acquire();
		arrayList.add(0, data);
		readSemaphore.release();
		writeLock.unlock();
	}
	
	public T readFromQueue() throws InterruptedException {
		readLock.lock();
		readSemaphore.acquire();
		T data = arrayList.get(writeSemaphore.availablePermits() - 1);
		writeSemaphore.release();
		readLock.unlock();
		return data;
		
	}
	
}