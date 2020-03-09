package test;

public class LastProd implements Runnable {

	@Override
	public void run() {
		int cnt = 0;
		while(true) {
			try {
				LastEx.semaphore.acquire(LastEx.numOfCons);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			LastEx.globalDataInteger +=1;
			
			LastEx.lock.lock();
			{
				LastEx.condition.signalAll();
			}
			LastEx.lock.unlock();
		}
	}

}
