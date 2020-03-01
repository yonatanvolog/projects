package il.co.ilrd.observer;

import java.util.function.Consumer;
import java.util.function.Supplier;

public class Callback <T>{
	private Dispatcher <T> dispatcher;
	
	private Consumer<T> updateCallback = null;
	private Consumer<T> stopUpdateCallback = null;

	public Callback() {}
	
	public Callback(Consumer<T> updateCallback, Consumer<T> stopUpdateCallback) {
		this.updateCallback = updateCallback;
		this.stopUpdateCallback = stopUpdateCallback;
	}
	
	public void update(T param) {
		
	}
	
	public void setUpdateCallback(Consumer<T> updateCallbackFuncConsumer) {
	}
	
	public void stopUpdate() {
		
	}
	public Dispatcher getDispatcher() {
		return dispatcher;
	}
	public void setDispatcher(Dispatcher dispatcher) {
		this.dispatcher = dispatcher;
	}
}
