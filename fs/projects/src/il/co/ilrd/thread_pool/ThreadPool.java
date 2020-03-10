package il.co.ilrd.thread_pool;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executor;
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
	private boolean toRun = true;
	
	public enum TaskPriority {
		MIN,
		NORM,
		MAX
	}
	
	public ThreadPool() {
		this(DEAFULT_NUM_THREADS);
	}
	
	public <T> ThreadPool(int num) {
		for (; num > 0; --num) {
			threadsList.add(new TPThread<T>());
			System.out.println("new thread " + num); //debug
		}
		
		for(TPThread<?> thread: threadsList) {
			thread.start();
		}
	}
	
	private class TPThread<T> extends Thread {
		@Override
		public void run() {
			while(toRun) {
				try {
					ThreadPoolTask task = tasksQueue.dequeue();
					Future taskFuture = task.runTask();
					//work in progress
				
				
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}
			System.out.println("running thread "); //debug
		}
	}
	
	public <T> Future<T> submitTask(Callable<T> callable) {
		return null;
	}
	
	public <T> Future<T> submitTask(Callable<T> callable, TaskPriority taskPriority) {
		return null;
	}
	
	public Future<Void> submitTask(Runnable runnable, TaskPriority taskPriority) {
		return null;
	}
	
	public <T> Future<T> submitTask(Runnable runnable, TaskPriority taskPriority, T t) {
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
	
	private class ThreadPoolTask<T> implements Comparable<T> {	
		private TaskPriority taskPriority;
		private Callable<T> callable;
		private TaskFuture taskFuture = new TaskFuture();
		private Semaphore runTaskSem = new Semaphore(0);
		
		
		public ThreadPoolTask(TaskPriority taskPriority, Callable<T> callable) {
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
				return isDone;
			}
			
		}
	}	
}