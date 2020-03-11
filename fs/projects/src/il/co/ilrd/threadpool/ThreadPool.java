package il.co.ilrd.threadpool;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executor;
import java.util.concurrent.Future;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import il.co.ilrd.WaitableQueueSem.WaitableQueueSem;
import il.co.ilrd.hashMap.HashMap;


public class ThreadPool implements Executor {	
	private final static int DEAFULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();
	private final static int MAX_PRIORITY = 10;
	private final static int MIN_PRIORITY = -10;
	
	private List<TPThread> threadsList = new ArrayList<>();
	private WaitableQueueSem<ThreadPoolTask<?>> tasksQueue = new WaitableQueueSem<>();
	private HashMap<Long, TPThread> threadTable = new HashMap<>();
	
	private Semaphore pauseSem = new Semaphore(0);
	private Semaphore terminateSem = new Semaphore(0);
	
	private int numOfThreads;
	private boolean IsTerminated = false;
	private boolean isTerminating = false;
	
	
	public enum TaskPriority {
		MIN(0),
		NORM(1),
		MAX(2);
		
		Integer val;
		private TaskPriority(int val) {
			this.val = val;
		}
		public int getVal() {
			return val;
		}
	}
	
	public ThreadPool() {
		this(DEAFULT_NUM_THREADS);
	}
	
	public <T> ThreadPool(int num) {
		numOfThreads = num;
		AddThreadsToList(numOfThreads);
	}


	private class TPThread extends Thread {
		boolean isKilled = false;
		
		@Override
		public void run() {
			ThreadPoolTask<?> task = null;

			while (!isKilled && !IsTerminated) {
				try {
					task = tasksQueue.dequeue();
					task.runTask();
				} catch (Exception e) {
					task.runTaskSem.release();
					task.taskFuture.exception = new ExecutionException(e);
					}
			}
		}
	}
	
	//------------------------------------------------------------------------------//
	
	@SuppressWarnings("hiding")
	private class WrapRunVoid<Void> implements Callable<Void>{
		Runnable runned;
		private WrapRunVoid(Runnable run){
			runned = run;
		}
		@Override
		public Void call() throws Exception {
			runned.run();
			return null;
		}
	}
	
	private class WrapRunVal<T> implements Callable<T>{
		Runnable runned;
		T t;
		private WrapRunVal(Runnable run, T t){
			runned = run;
			this.t = t;
		}
		@Override
		public T call() throws Exception {
			runned.run();
			return t;
		}
	}
	
	private class killMe implements Runnable{

		@Override
		public void run() {
			long id = Thread.currentThread().getId();
			threadTable.get(id).isKilled = true;
			threadTable.remove(id);
			threadsList.remove(Thread.currentThread());
			
			if(isTerminating) { 
				terminateSem.release();
			}
		}
	}
	
	private class pauseMe implements Runnable{

		@Override
		public void run() {
			try {pauseSem.acquire();} 
			catch (InterruptedException e) {e.printStackTrace();}
		}
	}
	
	private void AddKillingTasks(int num, int priority) {
		for(int i = 0; i < num ;i++) {
			submitTask(new killMe(), priority);
		}
	}

	private void AddThreadsToList(int num) {
		for (int i = 0; i < num; i++) {
			TPThread thread = new TPThread();
			threadsList.add(thread);
			threadTable.put(thread.getId(), thread);
			thread.start();
		}
	}
	private void CheckIfAlive() {
		if (IsTerminated) {
			throw new PoolTerminated();
		}
	}
	class PoolTerminated extends RuntimeException{
		private static final long serialVersionUID = 365623605979666811L;
		}
	
	//------------------------------------------------------------------------------//

	public <T> Future<T> submitTask(Callable<T> callable) {
		return submitTask(callable, TaskPriority.NORM);
	}
	
	public <T> Future<T> submitTask(Callable<T> callable, TaskPriority taskPriority) {
		return submitTask(callable, taskPriority.getVal());
	}
	
	public Future<Void> submitTask(Runnable runnable, TaskPriority taskPriority) {
		return submitTask(new WrapRunVoid<Void>(runnable), taskPriority);
	}
	
	public <T> Future<T> submitTask(Runnable runnable, TaskPriority taskPriority, T t) {
		return submitTask(new WrapRunVal<T>(runnable, t), taskPriority);
	}
	private Future<Void> submitTask(Runnable runnable, int priority) {
		return submitTask(new WrapRunVoid<Void>(runnable), priority);
	}
	
	private  <T> Future<T> submitTask(Callable<T> callable, int priority) {
		CheckIfAlive();
		ThreadPoolTask<T> task = new ThreadPoolTask<T>(priority, callable);
		tasksQueue.enqueue(task);
		
		return task.getFuture();
	}
	
	public void setNumberOfThread(int num) {
		if (num > numOfThreads) {
			AddThreadsToList(num - numOfThreads);
		}else {
			AddKillingTasks(-(num - numOfThreads), MAX_PRIORITY);
		}
		numOfThreads = num;
	}

	@Override
	public void execute(Runnable runnable) {
		submitTask(runnable, TaskPriority.NORM);
	}
	
	public void pause() {
		for (int i = 0; i < numOfThreads; i++) {
			submitTask(new pauseMe(), MAX_PRIORITY);
		}
	}
	
	public void resume() {
		pauseSem.release(numOfThreads);
	}
	
	public void shutdown() throws InterruptedException {
		isTerminating = true;
		AddKillingTasks(numOfThreads, MIN_PRIORITY);
		IsTerminated = true;
	}

	public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException{
			return terminateSem.tryAcquire(numOfThreads, timeout, unit);
	}
	
	private class ThreadPoolTask<T> implements Comparable<ThreadPoolTask<T>> {	
		private int intPriority;
		private Callable<T> callable;
		private TaskFuture taskFuture = new TaskFuture();
		private Semaphore runTaskSem = new Semaphore(0);
		private Lock cancelLock = new ReentrantLock();

		private boolean isCanceled = false;
		
		public ThreadPoolTask(int intPriority, Callable<T> callable) {
			this.intPriority = intPriority;
			this.callable = callable;
		}

		public TaskFuture getFuture() {
			return taskFuture;
		}

		@Override
		public int compareTo(ThreadPoolTask<T> task) {
			return  task.intPriority - intPriority;
		}
		
		private void runTask() throws Exception {
			cancelLock.lock();
			taskFuture.isRun = true;
			if (!isCanceled) {
				taskFuture.returnObj = callable.call();	
				taskFuture.isDone = true;
			}
			runTaskSem.release();
			cancelLock.unlock();
		}
		
		private class TaskFuture implements Future<T> {
			private boolean isDone = false;
			private boolean isRun = false;
			ExecutionException exception;

			T returnObj;
			
			@Override
			public boolean cancel(boolean arg0) {
			boolean removed = false;
				if (!isRun && cancelLock.tryLock()) {
					try {
						isCanceled = true;
						while (!removed) {
							removed = tasksQueue.remove(ThreadPoolTask.this);
						}
					} 
					catch (InterruptedException e) {e.printStackTrace();}
					cancelLock.unlock();

				}
				return isCanceled;
			}

			@Override
			public T get() throws InterruptedException, ExecutionException {
				runTaskSem.acquire();
				if (exception != null) {
					throw exception;
				}

				return returnObj;
			}

			@Override
			public T get(long quantity, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException {
				if (exception != null) {
					throw exception;
				}
				runTaskSem.tryAcquire(quantity, unit);
				return returnObj;
			}

			@Override
			public boolean isCancelled() {
				return isCanceled;
			}

			@Override
			public boolean isDone() {
				return isDone;
			}
			
		}
	}
	
	
}