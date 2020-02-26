package il.co.ilrd.singleton;

//We could synchronize only the "critical section", but in this situation 
//the treads just wind up waiting for each other to instantize the object

public class Singleton3 {
	public static Singleton3 instance;
	private Singleton3() {}
	
	public static Singleton3 getInstance() {
		if(null == instance) {
			synchronized (Singleton3.class) {
				instance = new Singleton3();
			}
		}
		return instance;
	}
}
