package il.co.ilrd.observer;

import java.util.function.Consumer;

public class Observer <T> {
	
	private Callback<T> callback;
	
	//Methods user wants to be performed on observer by subject on update
	static void imUpdated() {
		System.out.println("observer myUpdateFunc");
	}
	
	static void imStopUpdated() {
		System.out.println("observer myUpdateFunc");
	}
	
	private Consumer<T> myUpdateFunc = (T) -> imUpdated();
	private Consumer<T> myStopUpdateFunc = (T) -> imStopUpdated();
	
	public void register(Subject<T> subject) {
		callback = new Callback<T>(myUpdateFunc, myStopUpdateFunc);
		subject.register(callback);
	}
	public void unregister() {
		callback.getDispatcher().unregister(callback);
	}	
	public void unregister(Subject<T> subject) {
		subject.unregister(callback);
	}


}
