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
	private List<TPThread> threadsList = new ArrayList<>();
	private WaitableQueueSem<ThreadPoolTask<?>> tasksQueue = new WaitableQueueSem<>();
	private final static int DEAFULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();
	private int numOfThreads = 0;
	private boolean isShutdown = false;
	private Semaphore pauseSem = new Semaphore(0);
	private Semaphore shutdownSem = new Semaphore(0);
	private final static int VIP_PRIORITY = 100;
	private final static int PEASANT_PRIORITY = -100;
	
	public enum TaskPriority {
		MIN,
		NORM,
		MAX
	}
	
	public ThreadPool() {
		this(DEAFULT_NUM_THREADS);
	}
	
	private void AddAndStartThread() {
		TPThread newThread = new TPThread();
		threadsList.add(newThread);
		newThread.start();
		++numOfThreads;
	}
	
	public <T> ThreadPool(int num) {
		for (; num > 0; --num) {
			AddAndStartThread();
		}
	}
	
	private class TPThread extends Thread {
		private boolean toRun = true;

		@Override
		public void run() {
			ThreadPoolTask<?> currTask = null;
			
			while(toRun) {
				try {
					currTask = tasksQueue.dequeue();
					currTask.runTask();
					
				} catch (Exception taskException) {
					currTask.taskFuture.taskException = new ExecutionException(taskException);
					currTask.runTaskSem.release();
				}
			}		
			while(false == threadsList.remove(this)) {}
		}
	}
	
	private Future<Void> submitTaskGeneric(Runnable runnable, int taskPriority) {
		return submitTaskGeneric(Executors.callable(runnable, null), taskPriority);
	}
	
	private <T> Future<T> submitTaskGeneric(Callable<T> callable, int taskPriority) {
		if(isShutdown) {
			throw new ThreadPoolisShutdownException();
		}
		
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
	
	public void setNumberOfThread(int updatedThreadsNum) throws ThreadPoolisShutdownException, InterruptedException {
		if(updatedThreadsNum > numOfThreads) {
			for(int i = 0; i < updatedThreadsNum - numOfThreads; ++i) {
				AddAndStartThread();
			}
		} else {
			for(int i = 0; i < numOfThreads - updatedThreadsNum; ++i) {	
				submitTaskGeneric(new killThreadTask(), VIP_PRIORITY);
			}
		}
	}
	
	@Override
	public void execute(Runnable runnable) throws RuntimeException{
		submitTask(runnable, TaskPriority.NORM);	
	}
	
	public void pause() {
		for(int i = 0; i < numOfThreads; ++i) {	
			submitTaskGeneric(new pauseTask(), VIP_PRIORITY);
		}
	}
	
	public void resume() {
		pauseSem.release(numOfThreads);
	}
	
	public void shutdown() {
		for(int i = 0; i < numOfThreads; ++i) {	
			submitTaskGeneric(new shutdownTask(), PEASANT_PRIORITY);
		}
		
		isShutdown = true;
	}

	public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
		return shutdownSem.tryAcquire(numOfThreads, timeout, unit);
	}
	
	private class ThreadPoolTask<T> implements Comparable<ThreadPoolTask<T>> {	
		private int taskPriority;
		private Callable<T> callable;
		private TaskFuture taskFuture = new TaskFuture();
		private Semaphore runTaskSem = new Semaphore(0);

		public ThreadPoolTask(int taskPriority, Callable<T> callable) {
			this.taskPriority = taskPriority;
			this.callable = callable;
		}
		
		public TaskFuture getFuture() {
			return taskFuture;
		}

		@Override
		public int compareTo(ThreadPoolTask<T> other) {
			return other.taskPriority - this.taskPriority;
		}
		
		private void runTask() throws Exception {
			taskFuture.returnObj = callable.call();
			taskFuture.isDone = true;
			runTaskSem.release();
		}
		
		private class TaskFuture implements Future<T> {
			private boolean isDone = false;
			private boolean isCancelled = false;
			T returnObj;
			ExecutionException taskException = null;
			
			@Override
			public boolean cancel(boolean arg0) {
				try {
					if(tasksQueue.remove(ThreadPoolTask.this)) {
						isCancelled = true;
						isDone = true;
						
						return true;
					}					
				} catch (Exception e) {}
				
				return false;
			}

			@Override
			public T get() throws InterruptedException, ExecutionException {
				runTaskSem.acquire();

				if(null != taskException) {
					throw taskException;
				}
				
				return returnObj;
			}

			@Override
			public T get(long timeout, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException {
				if(runTaskSem.tryAcquire(timeout,unit)) {					
					if(null != taskException) {
						throw taskException;
					}

					return returnObj;
					
				} else {
					throw new TimeoutException();
				}
			}

			@Override
			public boolean isCancelled() {
				return isCancelled;
			}

			@Override
			public boolean isDone() {
				return isDone;
			}
		}
	}
	
	public class ThreadPoolisShutdownException extends RuntimeException { 
	    public ThreadPoolisShutdownException() {
	        super("ThreadPool isShutdown, cannot add new tasks");
	    }
	}
	
	private class pauseTask implements Runnable {
		@Override
		public void run() {
			try {
				pauseSem.acquire();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private class killThreadTask implements Runnable {
		@Override
		public void run() {
			TPThread currentThread = (TPThread)Thread.currentThread();
			currentThread.toRun = false;
			--numOfThreads;
		}
	}
	
	private class shutdownTask implements Runnable {
		@Override
		public void run() {
			TPThread currentThread = (TPThread)Thread.currentThread();
			currentThread.toRun = false;
			--numOfThreads;
			shutdownSem.release();			
		}
	}
}