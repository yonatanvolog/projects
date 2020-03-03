package il.co.ilrd.observer;

import java.util.LinkedList;
import java.util.List;

public class Dispatcher<T> {
	private List<Callback<T>> callbackList = new LinkedList<>();
	
	public void register(Callback<T> callback) {
		callback.setDispatcher(this);
		callbackList.add(callback);
	}
	public void unregister(Callback<T>  callback) {
		callbackList.remove(callback);
	}
	public void updateAll(T param) {
		for (Callback<T> callback : callbackList) {
			callback.update(param);
		}
	}
	
	public void stopUpdate() {
		for (Callback<T> callback : callbackList) {
			callback.stopUpdate();
		}
	}
}