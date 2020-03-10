package il.co.ilrd.threadpool;

import static org.junit.jupiter.api.Assertions.*;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;

import org.junit.jupiter.api.Test;


import il.co.ilrd.threadpool.ThreadPool.TaskPriority;

class threadPool {
	ThreadPool pool;
	Integer runtest = 5;

	
	@Test
	void test() throws Exception, ExecutionException {
		pool = new ThreadPool(1);
//		pool.submitTask( new testSubmitnorm(), TaskPriority.NORM);
//		pool.submitTask(new testSubmitlow(), TaskPriority.MIN);
//		System.out.println(pool.submitTask(new testSubmithigh(), TaskPriority.MAX).get());
//		pool.submitTask(new testRun(), TaskPriority.NORM);
		System.out.println(pool.submitTask(new testRunVal(), TaskPriority.MAX, runtest).get());
		System.out.println(pool.submitTask(new testRunVal(), TaskPriority.MAX, runtest).get());
		System.out.println(pool.submitTask(new testRunVal(), TaskPriority.MAX, runtest).get());
		System.out.println(pool.submitTask(new testRunVal(), TaskPriority.MAX, runtest).get());


		

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
				System.out.println("this a high");
				return 2;
		}
		
	}
}

