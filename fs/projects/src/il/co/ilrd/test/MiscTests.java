package il.co.ilrd.test;

class Second extends Thread {
	public void run() {
		System.out.println("thread2");
	}
}


public class MiscTests {

	public static void main(String[] args) {
		Thread t1 = new Thread(new Runnable() {
			
			@Override
			public void run() {
				System.out.println("theead1");
			}
		});
		
		Thread t2 = new Thread (new Runnable() {
			
			@Override
			public void run() {
				System.out.println("theead12222");
			}
		});
		
		t1.start();
		t2.start();
	}
}
