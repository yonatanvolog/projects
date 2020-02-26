package il.co.ilrd.singleton;

//This solution complements Singleton1, and it does work because the method
//getInstance is synchronized, which means only 1 thread at a time can execute
//it. The problem is if 1 thread executes the method, and another one is waiting
//to do the same, it will take a lot of time (in CPU cycles)

public class Singleton2 {
	public static Singleton2 instance;
	private Singleton2() {}
	
	public synchronized static Singleton2 getInstance() {
		if(null == instance) {
			instance = new Singleton2();
		}
		return instance;
	}
}
