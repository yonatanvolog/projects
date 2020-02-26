package java2c;

public class Dog extends Animal {
	public Dog() {
		super(2);
		System.out.println("Dog ctor");
	}
	static {
		System.out.println("Static block Dog");
	}
	
	public void sayHello() {
		System.out.println("Dog Hello!");
		System.out.println("I have " + num_legs + " legs");
	}
	{
		System.out.println("Instance initialization block Dog");
	}
	
	@Override
	protected void finalize() throws Throwable{
		System.out.println("finalize Dog with ID: " + this.ID);
		super.finalize();
	}
	
	@Override
	public String toString() {
		return "Dog with ID: " + ID;
	}
	
	private int num_legs = 4;
}
