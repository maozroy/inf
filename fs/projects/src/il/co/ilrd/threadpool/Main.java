package il.co.ilrd.threadpool;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

public class Main {
	int runtest = 0;
	public static void main(String[] args) throws InterruptedException, ExecutionException {
		ThreadPool pool = new ThreadPool(1);
		Future<Integer> future;
		future = pool.submitTask(new Main().new testSubmithigh());
		//Thread.sleep(1000);
		System.out.println(future.get());
		pool.setNumberOfThread(3);
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
			Thread.sleep(1);
				System.out.println("this a norm");
				return 0;
		}
		
	}
	class testSubmitlow implements Callable<Integer>{

		@Override
		public Integer call() throws Exception {
				System.out.println("this a low");
				return 1;
		}
		
	}
	class testSubmithigh implements Callable<Integer>{

		@Override
		public Integer call() throws Exception {
				for(int i = 0; i < 1000000 ; i++) {
				System.out.println("this a high" + i);
				}
				
				return 2;
		}
		
	}
}
