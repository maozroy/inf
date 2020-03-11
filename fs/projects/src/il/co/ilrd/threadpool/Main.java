package il.co.ilrd.threadpool;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import il.co.ilrd.threadpool.ThreadPool.TaskPriority;

public class Main {
	int runtest = 0;
	
	
	
	public static void main(String[] args) throws InterruptedException, ExecutionException {
		ThreadPool pool = new ThreadPool(0);
		Future<Integer> future;
		pool.submitTask(new Main().new testSubmitlow(), TaskPriority.MIN);
		pool.submitTask(new Main().new testSubmitnorm(), TaskPriority.NORM);
		pool.submitTask(new Main().new testSubmithigh(), TaskPriority.MAX);
		pool.setNumberOfThread(1);
		pool.shutdown();
		System.out.println(pool.awaitTermination(10, TimeUnit.SECONDS));


	//	pool.resume();
//		pool.submitTask(new Main().new testSubmitnorm());
//		future = pool.submitTask(new Main().new testSubmithigh());
//		pool.setNumberOfThread(3);
		
//		System.out.println(future.get());

	}
	
	class testRun implements Runnable{

		@Override
		public void run() {
			System.out.println("run test");			
		}
		
	}
	
	class testRunVal implements Runnable{

		@Override
		public void run() {
			System.out.println("run test");	
			runtest+=1;
		}
		
	}

	class testSubmitnorm implements Callable<Integer>{

		@Override
		public Integer call() throws Exception {
				System.err.println("this a norm");
				return 0;
		}
		
	}
	class testSubmitlow implements Callable<Integer>{

		@Override
		public Integer call() throws Exception {
				System.err.println("this a low");
				for (int i = 0; i < 1000000; i++) {
					if (i%1000 == 0) {
						System.out.println(i);
					}
				}
				return 1;
		}
		
	}
	class testSubmithigh implements Callable<Integer>{

		@Override
		public Integer call() throws Exception {
				System.err.println("this a high");
				
				
			return 2;
		}
		
	}
}
