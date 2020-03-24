package il.co.ilrd.test;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;


class A <T> {
	public void foo (Object o) {}
	public <Z extends Integer> void foo (Z t) {}
}



public class MiscTests {

	public static void main(String[] args) throws InterruptedException, ExecutionException {
		ExecutorService executorService = Executors.newFixedThreadPool(10);

		executorService.execute(new Runnable() {
		    public void run() {
		        System.out.println("Asynchronous task");
		    }
		});

	
	
		Future<String> result = executorService.submit(new RingRing<String>());
		
		executorService.shutdown();

		System.out.println(result.get());
		
		
		
		
		ExecutorService es1 = Executors.newSingleThreadExecutor();
		Future<String> result1 = es1.submit(new RingRing<String>());
		
		ExecutorService es2 = Executors.newFixedThreadPool(1);
		Future<String> result2 = es2.submit(new RingRing<String>());
		
		ExecutorService es3 = Executors.newCachedThreadPool();
		Future<String> result3 = es3.submit(new RingRing<String>());
		
		ExecutorService es4 = Executors.newScheduledThreadPool(5);
		Future<String> result4 = es4.submit(new RingRing<String>());
		
		System.out.println("a" + result1.get());
		System.out.println("b" + result2.get());
		System.out.println("c" + result3.get());
		System.out.println("d" + result4.get());


	}
}
