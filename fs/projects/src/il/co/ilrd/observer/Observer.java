package il.co.ilrd.observer;

import java.util.function.Consumer;

public class Observer <T> {
	
	private Callback<T> callback;
	
	static void imUpdated() {
		System.out.println("observer myUpdateFunc");
	}
	
	static void imStopUpdated() {
		System.out.println("observer myUpdateFunc");
	}
	
	private Consumer<T> myUpdateFunc = (T) -> imUpdated();
	private Consumer<T> myStopUpdateFunc = (T) -> imStopUpdated();

	
	public void regitser(Subject<T> subject) {
		callback = new Callback<T>(myUpdateFunc, myStopUpdateFunc);
		subject.register(callback);
	}
	public void unregitser() {
		
	}	
	public void unregitser(Subject<T> subject) {
		
	}
}
