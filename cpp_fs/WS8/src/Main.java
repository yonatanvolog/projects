import java.io.IOException;

public class Main {

	public static void main(String[] args) {
		
		try {
			Foo.func1();
		} catch (IOException e) {
			System.out.println("caught func1 IOException");
		}
		//Foo.func2();
	
		//Foo.func3();
		
		try {
			Foo.func4();
		} catch (MyException2 e) {
			System.out.println("caught func4 MyException2");
		}
		
		byte[] byteArray = new byte[100];
		//System.out.println(byteArray[101]);
		
		/*String className = “abc.xyz.Manager”;
		Class cls = Class.forName(className);
		Manager mgr = (Manager)cls.newInstance();
		mgr.someMethod();*/
		
        try {
			Class c1 = Class.forName("Demo");
		} catch (ClassNotFoundException e) {
			System.out.println("caught ClassNotFoundException");
		}
        
        Dummy1 d1 = new Dummy1();
        //Now open WS8 folder/bin and delete Dummy1.class
	}
}
