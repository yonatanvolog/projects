package il.co.ilrd.waitable_queue;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueSem<T> {
	
	PriorityQueue<T> queue;
	Semaphore sem = new Semaphore(0);
	Lock lock = new  ReentrantLock();
	
	public WaitableQueueSem() {
		this(null);
	}
	
	public WaitableQueueSem(Comparator<T> comp) {
		queue = new PriorityQueue<>(comp);
	}
	
	public void enqueue (T elem) {
		lock.lock();
		queue.add(elem);
		lock.unlock();
		sem.release();
	}

	public T dequeue() throws InterruptedException {		
		sem.acquire();
		lock.lock();
		T data = queue.poll();
		lock.unlock();
		
		return data;
	}
	
	public T dequeue(long timeout,  TimeUnit timeunit) throws InterruptedException {
		T data = null;

		if(sem.tryAcquire(timeout, timeunit)) {
			lock.lock();
			data = queue.poll();
			lock.unlock();
		}
		
		return data;
	}
	
	public boolean remove (T elem) throws InterruptedException {
		boolean removeRes = false;
		
		if(sem.tryAcquire()) {
			if(lock.tryLock()) {
				removeRes = queue.remove(elem);
				if(!removeRes) {
					sem.release();
				}
				lock.unlock();
			}
			else {
				sem.release();
			}
		}
		
		return removeRes;
	}
}