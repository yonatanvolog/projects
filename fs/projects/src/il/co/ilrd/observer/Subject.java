package il.co.ilrd.observer;

public class Subject<T> {
	
	private Dispatcher dispatcher = new Dispatcher<T>();
	
	public void register(Callback callback) {
		
	}
	public void unregister(Callback callback) {
		
	}
	public void updateAll() {
		
	}
	public void stopUpdate() {
		
	}
}
