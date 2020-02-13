package ilrd.vm;

import java.util.concurrent.atomic.AtomicBoolean;

public class TimeoutThread implements Runnable {
    private Thread timeoutThread;
    private final AtomicBoolean running = new AtomicBoolean(false);
	public static int timeoutCounter = 0;
	private VMachine vm;
	
	public TimeoutThread(VMachine vm) {
		this.vm = vm;
	}
	
	public void startTimer() {
		timeoutThread = new Thread(this);
		timeoutThread.start();
    }
	
	public void stopTimer() {
        running.set(false);
	}
	
	public void run(){
        running.set(true);
		while(running.get()) {
			++timeoutCounter;
			System.out.println("timeoutCounter is " + timeoutCounter);
			try {
				Thread.sleep(1 * 1000);
			} catch (InterruptedException e) {
				System.out.println("Thread sleep interrupted");
			}
			if(5 == timeoutCounter) {
				vm.notificationFromTimer();
			}
		}
	} 
}


