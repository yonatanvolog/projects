package java2c;

public class LegendaryAnimal extends Cat {
	public LegendaryAnimal() {
		System.out.println("Legendary Ctor");
		System.out.println("MAOZ TEST: super.toString() from legendary" + "****" + super.toString() + "****");
	}
	
	static {
		System.out.println("Static block LegendaryAnimal");
	}
	
	public void sayHello() {
		System.out.println("Legendary Hello!");
	}
	
	@Override
	protected void finalize() throws Throwable{
		System.out.println("finalize LegendaryAnimal with ID: " + this.ID);
		super.finalize();
	}
	
	@Override
	public String toString() {
		return "LegendaryAnimal with ID: " + ID;
	}
}
