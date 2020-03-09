package il.co.ilrd.WaitableQueueSem;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueSem <T>{
	private PriorityQueue<T> pqueue;
	private Semaphore semaphore = new Semaphore(0);
	private ReentrantLock lock = new ReentrantLock();
	
	public <C extends Comparable<T>>WaitableQueueSem() {
		this(null);
	}
	
	public WaitableQueueSem(Comparator<T> comp) {
		pqueue = new PriorityQueue<>(comp);
	}
	
	public void enqueue(T elem) {
		lock.lock();
		{
			pqueue.add(elem);
		}
		lock.unlock();
		semaphore.release();
	}
	
	public T dequeue() throws InterruptedException {		
		T valueT;
		
		semaphore.acquire();
		lock.lock();
		{	
			valueT = pqueue.poll();
		}
		lock.unlock();
		
		return valueT;
	}



	public T dequeue(long timeout, TimeUnit unit) throws InterruptedException {
		T valueT = null;

		if (semaphore.tryAcquire(timeout, unit)) {
			lock.lock();
			{	
				valueT = pqueue.poll();
			}
			lock.unlock();
		}
		
		return valueT;
	}
	
	public boolean remove(T elem) throws InterruptedException {
		boolean res = false;
		
		if (lock.tryLock()) {
			res = pqueue.remove(elem);
			
			if (res) {
				semaphore.acquire();
			}
			lock.unlock();
		}
		
		return res;
	}
}
