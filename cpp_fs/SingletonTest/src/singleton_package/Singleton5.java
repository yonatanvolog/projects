package singleton_package;

//The ultimate revision to this solution would be to make "instance" volatile,
//which means reading threads are guaranteed to see the latest value written 
//to the volatile variable

public class Singleton5 {
	public volatile static Singleton5 instance;
	private Singleton5() {}
	
	public static Singleton5 getInstance() {
		if(null == instance) {
			synchronized (Singleton5.class) {
				if(null == instance) {
					instance = new Singleton5();
				}
			}
		}
		return instance;
	}
}
