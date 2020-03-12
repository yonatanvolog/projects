package il.co.ilrd.thread_pool;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

class ThrowsException<V> implements Callable<V> {
	@Override
	public V call() throws Exception {
		Thread.sleep(2000);
		throw new Exception("some reason");
	}
}

class MyCallable<V> implements Callable<V> {
	String message;
	
	public void set(String message) {
		this.message = message;
	}
	
	@Override
	public V call() throws Exception {
		return (V) message;
	}
}

class MyRunnable<V> implements Runnable {
	String message;
	
	public void set(String message) {
		this.message = message;
	}
	
	@Override
	public void run() {
		System.err.println(message);
	}
}

class MyLongCallable<V> implements Callable<V> {

	@Override
	public V call() throws Exception {
		try {
			Thread.sleep(10); //10 in milliseconds

		} catch (InterruptedException e) {}
		return (V)"Long awaited result!";
	}
}

class RepeatablePrint<V> implements Runnable {
	String toPrint;
	int interval;
	
	public void set(String toPrint, int interval) {
		this.toPrint = toPrint;
		this.interval = interval;
	}
	@Override
	public void run() {
		for(int i = 0; i <= 100; ++i) {
			try {
				Thread.sleep(interval);
				System.err.println(toPrint + i);			

			} catch (InterruptedException e) {}
		}
	}
}

public class ThreadPoolTest {
	
	 //Future<T> submitTask(Callable<T> callable)
	public static void submit1Test() throws InterruptedException, ExecutionException {
		ThreadPool tp = new ThreadPool(3);
		
		MyCallable task1 = new MyCallable();
		task1.set("task1/3");
		
		MyCallable task2 = new MyCallable();
		task2.set("task2/3");
		
		MyCallable task3 = new MyCallable();
		task3.set("task3/3");
		
		Future<String> future1 = tp.submitTask(task1);
		Future<String> future2 = tp.submitTask(task2);
		Future<String> future3 = tp.submitTask(task3);
		
		System.out.println(future1.get());
		System.out.println(future2.get());
		System.out.println(future3.get());
	}

	//Future<T> submitTask(Callable<T> callable, TaskPriority taskPriority)
	public static void submit2Test() throws InterruptedException, ExecutionException {
		ThreadPool tp = new ThreadPool(3);
		
		MyCallable task1 = new MyCallable();
		task1.set("TaskPriority.MIN");
		
		MyCallable task2 = new MyCallable();
		task2.set("TaskPriority.NORM");
		
		MyCallable task3 = new MyCallable();
		task3.set("TaskPriority.MAX");
		
		Future<String> future1 = tp.submitTask(task1, ThreadPool.TaskPriority.NORM);
		Future<String> future2 = tp.submitTask(task2, ThreadPool.TaskPriority.MAX);
		Future<String> future3 = tp.submitTask(task3, ThreadPool.TaskPriority.MIN);
		
		System.out.println(future1.get());
		System.out.println(future2.get());
		System.out.println(future3.get());
	}

	//Future<Void> submitTask(Runnable runnable, TaskPriority taskPriority)
	public static void submit3Test() {
		ThreadPool tp = new ThreadPool(3);
		
		MyRunnable task1 = new MyRunnable();
		task1.set("task1/3");
		
		MyRunnable task2 = new MyRunnable();
		task2.set("task2/3");
		
		MyRunnable task3 = new MyRunnable();
		task3.set("task3/3");
		
		tp.submitTask(task1, ThreadPool.TaskPriority.NORM);
		tp.submitTask(task2, ThreadPool.TaskPriority.NORM);
		tp.submitTask(task3, ThreadPool.TaskPriority.NORM);
	}

	//Future<T> submitTask(Runnable runnable, TaskPriority taskPriority, T result)
	public static void submit4Test() throws InterruptedException, ExecutionException {
		ThreadPool tp = new ThreadPool(3);
		
		MyRunnable<String> task1 = new MyRunnable<>();
		task1.set("task1/3 printout");
		MyRunnable<String> task2 = new MyRunnable<>();
		task2.set("task2/3 printout");
		MyRunnable<String> task3 = new MyRunnable<>();
		task3.set("task3/3 printout");
		
		Future<String> future1 = tp.submitTask(task1, ThreadPool.TaskPriority.NORM, "task1/3");
		Future<String> future2 = tp.submitTask(task2, ThreadPool.TaskPriority.NORM, "task2/3");
		Future<String> future3 = tp.submitTask(task3, ThreadPool.TaskPriority.NORM, "task3/3");
		
		System.out.println(future1.get());
		System.out.println(future2.get());
		System.out.println(future3.get());
	}
	
	//void execute(Runnable runnable)
	public static void executeTest() {
		ThreadPool tp = new ThreadPool(3);
		
		MyRunnable task1 = new MyRunnable();
		task1.set("task1/3");
		
		MyRunnable task2 = new MyRunnable();
		task2.set("task2/3");
		
		MyRunnable task3 = new MyRunnable();
		task3.set("task3/3");
		
		tp.execute(task1);
		tp.execute(task2);
		tp.execute(task3);
	}

	public static void setNumberOfThreadDecreaseTest() throws Exception {
		ThreadPool tp = new ThreadPool(3);

		RepeatablePrint task1 = new RepeatablePrint();
		task1.set("taskA", 10);

		RepeatablePrint task2 = new RepeatablePrint();
		task2.set("taskB", 50);
		
		RepeatablePrint task3 = new RepeatablePrint();
		task3.set("taskC", 100);
		
		synchronized (tp) {
			tp.execute(task1);
			tp.execute(task2);
			tp.execute(task3);
		}
		

		Thread.sleep(1000);
		
		tp.setNumberOfThread(1);
		System.err.println("Num of threads set to 1, should complete tasks anyway");
		
		RepeatablePrint task4 = new RepeatablePrint();
		task4.set("FIRST", 200);
		
		RepeatablePrint task5 = new RepeatablePrint();
		task5.set("LAST", 200);
	
		synchronized (tp) {
			tp.execute(task4);
			tp.execute(task5);
		}
	}
	
	public static void setNumberOfThreadIncreaseTest() throws Exception {
		ThreadPool tp = new ThreadPool(1);

		RepeatablePrint task1 = new RepeatablePrint();
		task1.set("Only running task", 1);
		tp.execute(task1);

		Thread.sleep(1000);
		
		System.err.println("Num of threads set to 3, should start tasks");
		tp.setNumberOfThread(3);
		
		RepeatablePrint task2 = new RepeatablePrint();
		task2.set("taskA", 10);

		RepeatablePrint task3 = new RepeatablePrint();
		task3.set("taskB", 50);
		
		RepeatablePrint task4 = new RepeatablePrint();
		task4.set("taskC", 100);
		
		synchronized (tp) {
			tp.execute(task2);
			tp.execute(task3);
			tp.execute(task4);
		}
	}
	
	public static void pauseAndresumeTest() throws InterruptedException {
		ThreadPool tp = new ThreadPool(3);

		RepeatablePrint task1 = new RepeatablePrint();
		task1.set("taskA ", 100);
		
		tp.execute(task1);
		
		Thread.sleep(1000);
		tp.pause();
		
		RepeatablePrint task2 = new RepeatablePrint();
		task2.set("	taskB ", 200);

		tp.execute(task2);
		

		Thread.sleep(5000);
		System.err.println("		Ended sleep, now resuming:");

		tp.resume();
	}
	
	public static void shutdownTest() throws InterruptedException {
		ThreadPool tp = new ThreadPool(1);

		RepeatablePrint<?> task1 = new RepeatablePrint();
		task1.set("Only running task", 25);
		tp.submitTask(task1, ThreadPool.TaskPriority.NORM, "result");
		
		Thread.sleep(1000);
		System.err.println("shutting down");
		tp.shutdown();

		Thread.sleep(5000);
		
		RepeatablePrint<?> task2 = new RepeatablePrint();
		task2.set("taskA", 10);
		
		System.err.println("	executing new task");
		
		try {
			tp.submitTask(task2, ThreadPool.TaskPriority.NORM, "result");
		} catch (Exception e) {
			System.err.println("Caught expected exception! - GOOD!");
		}
	}
	
	public static void awaitTerminationTest() throws InterruptedException {
		ThreadPool tp = new ThreadPool(3);
		
		RepeatablePrint task1 = new RepeatablePrint();
		task1.set("taskA", 1);

		RepeatablePrint task2 = new RepeatablePrint();
		task2.set("taskB", 5);
		
		RepeatablePrint task3 = new RepeatablePrint();
		task3.set("taskC", 10);
		
		synchronized (tp) {
			tp.execute(task1);
			tp.execute(task2);
			tp.execute(task3);
		}
		
		tp.shutdown();
		
		if(false == tp.awaitTermination(10, TimeUnit.MILLISECONDS)) {
			System.err.println("	not terminated yet - GOOD");
		} else {
			System.err.println("	BAD RESULT!");
		}
		
		Thread.sleep(2000);
		
		if(true == tp.awaitTermination(100, TimeUnit.MILLISECONDS)) {
			System.err.println("	terminated - GOOD");
		} else {
			System.err.println("	BAD RESULT!");
		}
	}
	
	public static void getTest() throws InterruptedException { //exception
		ThreadPool tp = new ThreadPool(1);
		
		ThrowsException<?> task = new ThrowsException<>();
		
		Future<?> future = tp.submitTask(task, ThreadPool.TaskPriority.NORM);
		
		Thread.sleep(5000);
		
		try {
			future.get();
		} catch (Exception e) {
			System.err.println("future.get() threw Exception! - GOOD");
			System.err.println("The cause is: " + e.getCause());
		}	
	}
	
	public static void getTimedTest() throws InterruptedException { //exception
		ThreadPool tp = new ThreadPool(1);
		
		ThrowsException<?> task = new ThrowsException<>();
		
		Future<?> future = tp.submitTask(task, ThreadPool.TaskPriority.NORM);
		
		try {
			future.get(10, TimeUnit.MILLISECONDS);
		} catch (ExecutionException e1) {
			System.err.println("BAD RESULT!");
		} catch (TimeoutException e1) {
			System.err.println("timeouted, task not finished yet - GOOD!");
		}
		
		Thread.sleep(5000);

		try {
			future.get(10, TimeUnit.MILLISECONDS);
			
		} catch (ExecutionException e1) {
			System.err.println("future.get() threw Exception! - GOOD");
			System.err.println("The cause is: " + e1.getCause());
		} catch (TimeoutException e1) {
			System.err.println("BAD RESULT!");
		}
	
	}
	
	public static void cancelTest() throws InterruptedException {
		ThreadPool tp = new ThreadPool(1);

		tp.pause();
		
		RepeatablePrint<?> task1 = new RepeatablePrint();
		task1.set("Only running task", 25);
		Future<?> future1 = tp.submitTask(task1, ThreadPool.TaskPriority.NORM, "result");
		
		if(true == future1.cancel(false)) {
			System.out.println("successfully cencelled task1 - GOOD");
			System.out.println("true ?= " + (future1.isDone() == true));
			System.out.println("true ?= " + (future1.isCancelled() == true));
		} else {
			System.out.println("BAD RESULT!");
		}
		
		RepeatablePrint<?> task2 = new RepeatablePrint();
		task2.set("Only running task", 100);
		Future<?> future2 = tp.submitTask(task2, ThreadPool.TaskPriority.NORM, "result");
		
		tp.resume();
		
		Thread.sleep(1000);
		
		if(false == future2.cancel(false)) {
			System.out.println("task2 running, cannot cancel - GOOD!");
			System.out.println("true ?= " + (future2.isDone() == false));
			System.out.println("true ?= " + (future2.isCancelled() == false));
		} else {
			System.out.println("BAD RESULT!");
		}
	}
	
	public static void isDoneTest() throws InterruptedException {
		ThreadPool tp = new ThreadPool(1);

		RepeatablePrint<?> task1 = new RepeatablePrint();
		task1.set("Only running task", 50);
		Future<?> future = tp.submitTask(task1, ThreadPool.TaskPriority.NORM, "result");

		
		System.err.println("is done? false ?= " + future.isDone());

		Thread.sleep(5000);
		
		System.err.println("is done? false ?= " + future.isDone());

		Thread.sleep(1000);

		System.err.println("is done? true ?= " + future.isDone());
	}
	
	public static void main(String[] args) throws Exception {
		//submit1Test();
		//submit2Test();
		//submit3Test();
		//submit4Test();
		//executeTest();
		//setNumberOfThreadDecreaseTest();
		//setNumberOfThreadIncreaseTest();
		//pauseAndresumeTest();
		//shutdownTest();
		//awaitTerminationTest();
		//getTest();
		//getTimedTest();
		//cancelTest();
		//isDoneTest();
	}
}
