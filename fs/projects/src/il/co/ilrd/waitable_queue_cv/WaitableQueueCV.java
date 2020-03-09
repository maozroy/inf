package il.co.ilrd.waitable_queue_cv;


import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueCV <T>{
	private PriorityQueue<T> pqueue;
	private ReentrantLock lock = new ReentrantLock();
	Condition condition = lock.newCondition(); 
	
	public <C extends Comparable<T>>WaitableQueueCV() {
		this(null);
	}
	
	public WaitableQueueCV(Comparator<T> comp) {
		pqueue = new PriorityQueue<>(comp);
	}
	
	public void enqueue(T elem) {
		lock.lock();
		{
			pqueue.add(elem);
			condition.signalAll();
		}
		lock.unlock();
	}
	
	public T dequeue() throws InterruptedException {		
		T valueT;
		
		lock.lock();
		{
			while (pqueue.isEmpty()) {
				condition.await();
			}
			
			valueT = pqueue.poll();	
		}
		lock.unlock();
		
		return valueT;
	}

	public T dequeue(long timeout, TimeUnit unit) throws InterruptedException {
		
		T valueT = null;
		
		lock.lock();
		{
			if (pqueue.isEmpty()) {
				while(condition.await(timeout, unit)){}
			}
			valueT = pqueue.poll();	
		}
		lock.unlock();
		
		return valueT;		
	}
	
	public boolean remove(T elem) throws InterruptedException {
		boolean res = false;
		
		if (lock.tryLock()) {
			res = pqueue.remove(elem);
			lock.unlock();
		}
		return res;
	}

}
