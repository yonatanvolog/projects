package il.co.ilrd.factory;

import java.util.*;
import java.util.function.Function;

public class FatctoryTest {

	public static void main(String[] args) {
		Factory<Animal, String, Object> animalFactory = new Factory<>();

		
		/*************/System.out.println("Lambda Factory:");/*****************/
		Function<Object, Animal> dogCtor = (Dog) -> new Dog();
		animalFactory.add("Dog", dogCtor);
		Animal dogInst = animalFactory.create("Dog", null);
		
		dogInst.sayHello();
		
		/************/System.out.println("Anonymous Factory:");/***************/
		animalFactory.add("SnoopDog", new Function<Object, Animal>(){
			
			@Override
			public Animal apply(Object data) {
				return new SnoopDog();
			}
		});
		
		Animal snoopInst = animalFactory.create("SnoopDog", null);
		snoopInst.sayHello();

		/**************/System.out.println("Static Factory:");/****************/		
		animalFactory.add("sCat", Cat::staticCreateCat);
		Animal staticCatInst = animalFactory.create("sCat", null);
		staticCatInst.sayHello();
		
		/**************/System.out.println("Non Static Factory:");/************/		
		animalFactory.add("nsCat", new Cat()::nonStaticCreateCat);
		Animal nonStaticCatInst = animalFactory.create("nsCat", null);
		nonStaticCatInst.sayHello();
		
//		/**************/System.out.println("Particular Factory:");/************/
//		List<Cat> catList = new ArrayList<>();
//		catList.add(new Cat());
//		animalFactory.add("nCat", new Cat());
//
//		for(Cat arbitraryCat : catList) {
//			animalFactory.add("pCat", Cat::nonStaticCreateCat);
//		}
//		Animal someCat = animalFactory.create("pCat", null);
//		someCat.sayHello();
//		//very similar to a reference to a static method
	}
}