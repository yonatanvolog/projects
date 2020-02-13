package singleton_package;

//This is the simplest method, which isn't thread safe because one thread can
//check if instance is if, pause, let the other thread do the same and now
//they are both are about to instantize the object

public class Singleton1 {
	public static Singleton1 instance;
	private Singleton1() {}
	
	public static Singleton1 getInstance() {
		if(null == instance) {
			instance = new Singleton1();
		}
		return instance;
	}
}
