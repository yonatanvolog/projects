package il.co.ilrd.thread_pool;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import il.co.ilrd.waitable_queue.WaitableQueueSem;

public class ThreadPool implements Executor {	
	/*not sure about the list type (maybe list of Thread)*/
	private List<TPThread<?>> threadsList = new ArrayList<>();
	private WaitableQueueSem<ThreadPoolTask<?>> tasksQueue = new WaitableQueueSem<>();;
	private final static int DEAFULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();
	private int numOfThreads;
	private final static int VIP_PRIORITY = 100;

	
	public enum TaskPriority {
		MIN,
		NORM,
		MAX
	}
	
	public ThreadPool() {
		this(DEAFULT_NUM_THREADS);
	}
	
	
	private void AddAndStartThread() {
		TPThread<?> newThread = new TPThread<>();
		threadsList.add(newThread);
		newThread.start();	
	}
	
	public <T> ThreadPool(int num) {
		numOfThreads = num;
		
		for (; num > 0; --num) {
			AddAndStartThread();
			System.out.println("new thread " + num); //debug
		}
		
//		for (; num > 0; --num) {
//			threadsList.add(new TPThread<T>());
//			System.out.println("new thread " + num); //debug
//		}
//		
//		for(TPThread<?> thread: threadsList) {
//			thread.start();
//		}
	}
	
	private class TPThread<T> extends Thread {
		private boolean toRun = true;

		@Override
		public void run() {
			while(toRun) {
				try {
					tasksQueue.dequeue().runTask();
					
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}
			System.out.println("running thread "); //debug
		}
	}
	
	private <T> Future<T> submitTaskGeneric(Callable<T> callable, int taskPriority) {
		ThreadPoolTask<T> newTask = new ThreadPoolTask<T>(taskPriority, callable);
		tasksQueue.enqueue(newTask);
		
		return newTask.getFuture();
	}
	
	public <T> Future<T> submitTask(Callable<T> callable) {
		return submitTaskGeneric(callable, TaskPriority.NORM.ordinal());
	}
	
	public <T> Future<T> submitTask(Callable<T> callable, TaskPriority taskPriority) {
		return submitTaskGeneric(callable, taskPriority.ordinal());
	}
	
	public Future<Void> submitTask(Runnable runnable, TaskPriority taskPriority) {
		Callable<Void> callableObj = Executors.callable(runnable, null);
		
		return submitTaskGeneric(callableObj, taskPriority.ordinal());
	}
	
	public <T> Future<T> submitTask(Runnable runnable, TaskPriority taskPriority, T result) {
		Callable<T> callableObj = Executors.callable(runnable, result);
		
		return submitTaskGeneric(callableObj, taskPriority.ordinal());
	}
	
	
	public void setNumberOfThread(int updatedThreadsNum) {
		if(updatedThreadsNum > this.numOfThreads) {
			for(int i = 0; i < updatedThreadsNum - this.numOfThreads; ++i) {
				AddAndStartThread();
			}
		} else {
			class stopTask<T> implements Callable<T> {
				@Override
				public T call() throws Exception {
					TPThread<?> currentThread = (TPThread<?>)Thread.currentThread();
					currentThread.toRun = false;
					System.err.println("stopTaskStopped task");
					return null;
				}
			}
			
			for(int i = 0; i < this.numOfThreads - updatedThreadsNum; ++i) {	
				this.submitTaskGeneric(new stopTask<Void>(), VIP_PRIORITY);
			}
		}
	}
	
	@Override
	public void execute(Runnable runnable) {//exceptions?
		submitTask(runnable, TaskPriority.NORM);
	}
	
	public void pause() {
		
	}
	
	public void resume() {
		
	}
	
	public void shutdown() {
		
	}

	public void awaitTermination() {
		
	}
	
	private class ThreadPoolTask<T> implements Comparable<T> {	
		private int taskPriority;
		private Callable<T> callable;
		/*private*/ TaskFuture taskFuture = new TaskFuture();
		private Semaphore runTaskSem = new Semaphore(0);

		
		
		public ThreadPoolTask(int taskPriority, Callable<T> callable) {
			
			
			this.taskPriority = taskPriority;
			this.callable = callable;
		}
		
		public TaskFuture getFuture() {
			return taskFuture;
		}

		@Override
		public int compareTo(T arg0) {
			// TODO Auto-generated method stub
			return 0;
		}
		
		private void runTask() throws Exception {
			taskFuture.returnObj = callable.call();
			taskFuture.isDone = true;
			runTaskSem.release();
		}
		
		private class TaskFuture implements Future<T> {
			private boolean isDone = false;
			T returnObj;
			
			@Override
			public boolean cancel(boolean arg0) {
				// TODO Auto-generated method stub
				return false;
			}

			@Override
			public T get() throws InterruptedException, ExecutionException {
				runTaskSem.acquire();
				
				return returnObj;
			}

			@Override
			public T get(long timeout, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException {
				if(runTaskSem.tryAcquire(timeout,unit)) {
					return returnObj;
					
				} else {
					throw new TimeoutException();
				}
			}

			@Override
			public boolean isCancelled() {
				// TODO Auto-generated method stub
				return false;
			}

			@Override
			public boolean isDone() {
				return isDone;
			}
			
		}
	}	
}