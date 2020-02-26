package il.co.ilrd.singleton;

//This method is thread safe, as the instance will be creates as soon as the 
//class is loaded into memory and since its static, the line would'nt repeat.
//It's not lazy intantized though (for that you have to create the instance 
//only when you need

public class Singleton6 {
private Singleton6() {}

public static final Singleton6 instance = new Singleton6();
	public static Singleton6 getInstance() {
		return instance;
	}
}
