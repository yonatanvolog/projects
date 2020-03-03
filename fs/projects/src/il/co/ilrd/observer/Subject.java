package il.co.ilrd.observer;

public class Subject<T> {
	
	private Dispatcher<T> dispatcher = new Dispatcher<T>();
	
	public void register(Callback<T> callback) {
		dispatcher.register(callback);
	}
	public void unregister(Callback<T> callback) {
		dispatcher.unregister(callback);
	}
	public void updateAll() {
		dispatcher.updateAll(null);
	}
	public void stopUpdate() {
		dispatcher.stopUpdate();
	}
}
