package il.co.ilrd.WaitableQueueSem;

import java.util.Comparator;

public class QueueTest {
	static WaitableQueueSem<Integer> pq;
	volatile Integer cnt = 0;
	
	public static void main(String[] args) {
		pq = new WaitableQueueSem<>(new Comp());
		QueueTest qTest = new QueueTest();
		enq enq = qTest.new enq();
		deq deq = qTest.new deq();
		Thread t1 = new Thread(enq);
		Thread t10 = new Thread(enq);
		Thread t100 = new Thread(enq);

		Thread t2 = new Thread(deq);
		Thread t20 = new Thread(deq);
		Thread t200 = new Thread(deq);

		t1.start();
		t10.start();
		t100.start();

		t2.start();
		t20.start();
		t200.start();


		
	}
	
	static class Comp implements Comparator<Integer>{

		@Override
		public int compare(Integer arg0, Integer arg1) {
			return arg1 - arg0;
		}
		
	}
	
	class enq implements Runnable{
		@Override
		public void run() {
			while (true) {
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				pq.enqueue(++cnt);
			//	System.out.println(cnt +"in "+ Thread.currentThread());
				
			}
		}
		
	}
	class deq implements Runnable{
		Integer Res = 0;
		@Override
		public void run() {
			while (true) {
				
				try {
					Res =pq.dequeue(); 
					System.err.println(Res +"out " + Thread.currentThread());
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				try {
					Thread.sleep(Res * 10);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
	}

}
