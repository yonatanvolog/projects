package il.co.ilrd.factory;

import java.util.Map;
import java.util.function.Function;

import il.co.ilrd.hash_map.HashMap;

public class Factory<T, K, D> {
	private Map<K, Function<D, ? extends T>> map 
										  = new HashMap<>();
	
	public void add(K key, Function<D, ? extends T> func) {
		map.put(key, func);
	}

	public T create(K key, D data) {
		return map.get(key).apply(data);
	}
	
	public T create(K key) {
		return map.get(key).apply(null);
	}
	
//	public void add(String key, Object cat) {
//		map.put(key, cat);  //what to do with this?	
//	}
}

abstract class Animal {
	void sayHello() {
		System.out.println("Animal Hello");
	}
}

class Dog extends Animal {
	void sayHello() {
		System.out.println("Dog Hello");
	}
}

class SnoopDog extends Dog {
	void sayHello() {
		System.out.println("SnoopDog Hello");
	}	
}

class Cat extends Animal {
	void sayHello() {
		System.out.println("Cat Hello");
	}
	static Animal staticCreateCat(Object data) {
		return new Cat();
	}
	Animal nonStaticCreateCat(Object data) {
		return new Cat();
	}
}