package il.co.ilrd.waitable_queue;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueCondVar<T> {
	
	PriorityQueue<T> queue;
	Lock lock = new  ReentrantLock();
	Condition condition = lock.newCondition();
	
	public WaitableQueueCondVar() {
		this(null);
	}
	
	public WaitableQueueCondVar(Comparator<T> comp) {
		queue = new PriorityQueue<>(comp);
	}
	
	public void enqueue (T elem) {
		lock.lock();
		queue.add(elem);
		condition.signalAll();
		lock.unlock();
	}

	public T dequeue() throws InterruptedException {		
		lock.lock();
		while(queue.isEmpty()) {
			condition.await();
		}
		
		T data = queue.poll();
		lock.unlock();
		
		return data;
	}
	
	public T dequeue(long timeout,  TimeUnit timeunit) throws InterruptedException {
		lock.lock();
		while(queue.isEmpty()) {
			if(!condition.await(timeout, timeunit)) {
				lock.unlock();
				return null;
			}
		}
		T data = queue.poll();
		lock.unlock();

		return data;
	}
	
	public boolean remove (T elem) throws InterruptedException {
		boolean removeRes = false;
		
		if(lock.tryLock()) {
			removeRes = queue.remove(elem);
			lock.unlock();
		}
		
		return removeRes;
	}
}