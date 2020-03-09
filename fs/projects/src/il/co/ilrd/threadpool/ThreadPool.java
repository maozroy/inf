package il.co.ilrd.threadpool;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executor;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import il.co.ilrd.WaitableQueueSem.WaitableQueueSem;

public class ThreadPool<T> implements Executor {
	
	private List<Thread> threadsList = new ArrayList<Thread>();
	private WaitableQueueSem<ThreadPoolTask> tasksQueue = new WaitableQueueSem<ThreadPoolTask>();;
	private final static int DEAFULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();
	private int numOfThreads;
	
	public enum TaskPriority {
		MIN,
		NORM,
		MAX
	}
	
	public ThreadPool() {
		this(DEAFULT_NUM_THREADS);
	}
	
	public ThreadPool(int num) {
		
	}
	
	public Future<T> submitTask(Callable<T> callable) {
		return null;
	}
	
	public Future<T> submitTask(Callable<T> callable, TaskPriority taskPriority) { /* this is the main method*/
		return null;
	}
	
	public Future<Void> submitTask(Runnable runnable, TaskPriority taskPriority) {
		return null;
	}
	
	public Future<T> submitTask(Runnable runnable, TaskPriority taskPriority, T t) {
		return null;
	}
	
	public void setNumberOfThread(int num) {
		
	}
	
	@Override
	public void execute(Runnable runnable) {
		
	}
	
	public void pause() {
		
	}
	
	public void resume() {
		
	}
	
	public void shutdown() {
		
	}

	public void awaitTermination() {
		
	}
	
	public class ThreadPoolTask implements Comparable<T> {
		
		TaskPriority taskPriority;
		Callable<T> callable;
		
		public ThreadPoolTask(TaskPriority taskPriority, Callable<T> callable) {
			this.taskPriority = taskPriority;
			this.callable = callable;
		}
		
		@Override
		public int compareTo(T arg0) {
			// TODO Auto-generated method stub
			return 0;
		}
	}
	
	public class TaskFuture implements Future<T> {

		@Override
		public boolean cancel(boolean arg0) {
			// TODO Auto-generated method stub
			return false;
		}

		@Override
		public T get() throws InterruptedException, ExecutionException {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public T get(long arg0, TimeUnit arg1) throws InterruptedException, ExecutionException, TimeoutException {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public boolean isCancelled() {
			// TODO Auto-generated method stub
			return false;
		}

		@Override
		public boolean isDone() {
			// TODO Auto-generated method stub
			return false;
		}
		
	}
}