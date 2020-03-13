package il.co.ilrd.thread_pool;

import java.util.Objects;
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
	private WaitableQueueSem<ThreadPoolTask<?>> tasksQueue = new WaitableQueueSem<>();
	private final static int DEAFULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();
	private int numOfThreads = 0;
	private boolean isShutdown = false;
	private Semaphore pauseSem = new Semaphore(0);
	private Semaphore shutdownSem = new Semaphore(0);
	private final static int OFFSET = 100;
	private final static int VIP_PRIORITY = TaskPriority.HIGHEST_PRIORITY.ordinal() + OFFSET;
	private final static int PEASANT_PRIORITY = TaskPriority.LOWEST_PRIORITY.ordinal() - OFFSET;

	public enum TaskPriority {
		LOWEST_PRIORITY,
		MIN,
		NORM,
		MAX,
		HIGHEST_PRIORITY,
	}
	
	public ThreadPool() {
		this(DEAFULT_NUM_THREADS);
	}
	
	private void AddAndStartThread() {
		new TPThread().start();
		++numOfThreads;
	}
	
	public <T> ThreadPool(int num) {
		if(num < 0) {
			throw new IllegalArgumentException("Cannot create negative number of threads"); 
		}
		
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
					} catch (InterruptedException e) {
						//cannot do anything about WQ fail
					}
					currTask.runTask();
			}
		}
	}
	
	private Future<Void> submitTaskGeneric(Runnable runnable, int taskPriority) {
		return submitTaskGeneric(Executors.callable(runnable, null), taskPriority);
	}
	
	private <T> Future<T> submitTaskGeneric(Callable<T> callable, int taskPriority) {
		Objects.requireNonNull(callable);
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
		Objects.requireNonNull(unit);
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
			Objects.requireNonNull(other);
			return other.taskPriority - this.taskPriority;
		}
		
		private void runTask() {
			try {
				taskFuture.returnObj = callable.call();
			} catch (Exception taskException) {
				taskFuture.taskException = new ExecutionException(taskException);
			}
			
			taskFuture.isDone = true;
			runTaskSem.release();
		}
		
		private class TaskFuture implements Future<T> {
			private boolean isDone = false;
			private boolean isCancelled = false;
			private T returnObj;
			ExecutionException taskException = null;
			
			@Override
			public boolean cancel(boolean mayInterruptIfRunning) {
				try {
					if(tasksQueue.remove(ThreadPoolTask.this)) {
						isCancelled = true;
						isDone = true;
						runTaskSem.release();
						
						return true;
					}					
				} catch (Exception e) {}
				
				return false;
			}

			@Override
			public T get() throws InterruptedException, ExecutionException {
				try {
					get(Integer.MAX_VALUE, TimeUnit.DAYS);
				} catch (TimeoutException e) {
					//MAX_VALUE, cannot timeout
				}
				
				return returnObj;
			}

			@Override
			public T get(long timeout, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException {
				Objects.requireNonNull(unit);

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
