package il.co.ilrd.waitable_queue;

import java.util.concurrent.TimeUnit;

public class WaitableQueueSemTest {

	public static void main(String[] args) {
		WaitableQueueSem<Integer> queue = new WaitableQueueSem<>();
		
		Thread dequeueT = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					while(true) {
						Thread.sleep(1);
						queue.dequeue();
						System.out.println("	dequeue");
					}
				}
				catch (InterruptedException e) {
					e.printStackTrace();
				}
				
			}
		});
		
		Thread enqueueT = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					while(true) {
						Thread.sleep(1000);
						queue.enqueue(1);
						System.out.println("enqueue");
					}
				}
				catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		});

		
		
		//dequeueT.start();	
		//enqueueT.start();

		
		Thread dequeueTimedT = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					while(true) {
						Integer data = queue.dequeue(1000, TimeUnit.MILLISECONDS);
						
						if(null == data) {
							System.err.println("	didnt dequeue");
						} else {
							System.out.println("	dequeue");
						}
					}
				}
				catch (Exception e) {
					e.printStackTrace();
				}	
			}
		});
		
		
		//dequeueTimedT.start();
		

		
		Thread removeT = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					while(true) {
						boolean removeRes = queue.remove((Integer)1);
						
						if (false == removeRes) {
							System.err.println("	didnt remove");
						} else {
							System.out.println("	dequeue");
						}
					}
				}
				catch (Exception e) {
					e.printStackTrace();
				}	
			}
		});
		
		
		//removeT.start();	
	}
}
