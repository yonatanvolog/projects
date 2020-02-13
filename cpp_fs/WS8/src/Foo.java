import java.io.IOException;

public class Foo {
	public static void func1() throws IOException {
	    throw new IOException();
	}
	
	public static void func2() {
	    throw new NullPointerException();
	}
	
	public static void func3() throws MyException1 {
		throw new MyException1();
	}
	
	public static void func4() throws MyException2 {
		throw new MyException2();
	}
}
