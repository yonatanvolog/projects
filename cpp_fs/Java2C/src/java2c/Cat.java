package java2c;

public class Cat extends Animal	{
	public Cat() {
		this("black");
		System.out.println("Cat ctor");
		this.num_masters = 2;
	}
	static {
		System.out.println("Static block Cat");
	}
	public Cat(String colors) {
		this.colors = colors;
		System.out.println("Cat Ctor with color: " + this.colors);
	}
	@Override
	protected void finalize() throws Throwable{
		System.out.println("finalize Cat with ID: " + this.ID);
		super.finalize();
	}
	
	@Override
	public String toString() {
		return "Cat with ID: " + ID;
	}
	
	private String colors;
	private int num_masters = 5;
}