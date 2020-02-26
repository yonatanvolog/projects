package il.co.ilrd.singleton;

//In order to make sure we dont instantize the object if there is already one, 
//we could add a check inside the synchronised block.
//At first glance this method will work, because by the time the first thread 
//has finished its business in the synchrinised block, the second one would enter
//to find out its not null, and would do anything.
//The probelm with this solution is that the compiler is free to assign a value
//to the instance member before the constructor is called.
//If that happens, the first thread will get into the inner if block and assign
//a value to "instance" (without performing new), and the second thread would 
//check the outer if, see it's not null and return "instance", which isnt actually
//instantized (it was just about to do so)

public class Singleton4 {
	public static Singleton4 instance;
	private Singleton4() {}
	
	public static Singleton4 getInstance() {
		if(null == instance) {
			synchronized (Singleton4.class) {
				if(null == instance) {
					instance = new Singleton4();
				}
			}
		}
		return instance;
	}
}
