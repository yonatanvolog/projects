package java2c;

public class Java2c {
	public static void foo(Animal a) {
		System.out.println(a.toString());
	}
	@SuppressWarnings("deprecation")
	@Deprecated
		public static void main(String[] args) {
		Object obj;
		Animal animal = new Animal();
		Dog dog = new Dog();
		Cat cat = new Cat();

		LegendaryAnimal la = new LegendaryAnimal();
		
		Animal.showCounter();
		
		System.out.println(animal.ID);
		System.out.println(((Animal)dog).ID);
		System.out.println(((Animal)cat).ID);
		System.out.println(((Animal)la).ID);
		
		Animal[] array = {
				new Dog(),
				new Cat(),
				new Cat("white"),
				new LegendaryAnimal(),
				new Animal()
		};
		
		for(Animal a : array) {
			a.sayHello();
			System.out.println(a.getNumMasters());
		}
		
		for (Animal a : array) {
			foo(a);
		}
		
		System.gc();
		System.out.println("After gc");
		
		try {
			array[3].finalize();
		} catch (Throwable e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
