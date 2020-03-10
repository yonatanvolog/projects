package il.co.ilrd.thread_pool;

import java.awt.Robot;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

class MyCallable<V> implements Callable<V> {

	@Override
	public V call() throws Exception {
		Thread.sleep(1000);
		return (V) "MyCallable returned";
	}
}

class MyRunnable<V> implements Runnable {

	@Override
	public void run() {
		try {
			Thread.sleep(2000);
			System.err.println("MyRunnable printed out");

		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

class MyLongCallable<V> implements Callable<V> {

	@Override
	public V call() throws Exception {
		try {
			Thread.sleep(10); //10 in milliseconds

		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
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

			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}

public class ThreadPoolTest {
	public static void main(String[] args) throws Exception {
		ThreadPool tp = new ThreadPool(6);
		Future<String> myCallableFuture = tp.submitTask(new MyCallable<String>());
		System.out.println(myCallableFuture.get());
		
		
		Future<String> myRunnableFuture = tp.submitTask(new MyRunnable<String>(), ThreadPool.TaskPriority.NORM, "MyRunnable result");
		System.out.println(myRunnableFuture.get());
		
		Future<String> myLongCallableFuture = tp.submitTask(new MyLongCallable<String>(), ThreadPool.TaskPriority.MIN);
		
		try { 
			String res = myLongCallableFuture.get(5, TimeUnit.MILLISECONDS);
		} catch (TimeoutException e){
			System.err.println("get timed out - GOOD (expected result)");
		}
		
		System.out.println("should be string : " + myLongCallableFuture.get(1000, TimeUnit.MILLISECONDS));	//now should have result

		//tp.setNumberOfThread(0); shouldnt proceed all - works!
		
		System.out.println("POOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
		
		ThreadPool tp2 = new ThreadPool(3);
		
		RepeatablePrint<?> a = new RepeatablePrint();
		a.set("AAA", 10);
		
		RepeatablePrint<?> b = new RepeatablePrint();
		b.set("BBB", 30);
		
		RepeatablePrint<?> c = new RepeatablePrint();
		c.set("CCC", 100);
		
		tp2.execute(a);
		tp2.execute(b);
		tp2.execute(c);

		tp2.setNumberOfThread(2);

		
		//Thread.sleep(10000);
		
	}

}
