package singleton_package;

public class Main {

	public static void main(String[] args) {
		Singleton1 s1 = Singleton1.getInstance();
		System.out.println("Singleton1: " + s1.toString());
		
		Singleton2 s2 = Singleton2.getInstance();
		System.out.println("Singleton2: " + s2.toString());
	
		Singleton3 s3 = Singleton3.getInstance();
		System.out.println("Singleton3: " + s3.toString());
		
		Singleton4 s4 = Singleton4.getInstance();
		System.out.println("Singleton4: " + s4.toString());
		
		Singleton5 s5 = Singleton5.getInstance();
		System.out.println("Singleton5: " + s5.toString());
		
		Singleton6 s6 = Singleton6.getInstance();
		System.out.println("Singleton6: " + s6.toString());
		
		Singleton7 s7 = Singleton7.getInstance();
		System.out.println("Singleton7: " + s7.toString());
	}
}
