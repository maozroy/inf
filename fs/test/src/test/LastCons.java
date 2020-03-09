package test;

public class LastCons implements Runnable {

	@Override
	public void run() {
		while(true) {
			LastEx.lock.lock();
			{
				LastEx.semaphore.release();
				LastEx.condition.awaitUninterruptibly();
			}
			LastEx.lock.unlock();
			System.out.println(LastEx.globalDataInteger);
			
			}
		}		
	}


