package il.co.ilrd.jarloader;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.List;

public class TestJarLoader {

	public static void main(String[] args) throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
		String interfaceName = "SayHi";
		String jarPath = "/home/yonatan/Yonatan-Vologdin/fs/projects/bin/AnimalSounds.jar";
		
		List<Class<?>> loader = JarLoader.load(interfaceName, jarPath);
		
		System.out.println("These are the classes that impement interface - " + interfaceName + ":");
		for (Class<?> class1 : loader) {
			Constructor<?> constructorStr = class1.getConstructor();
			SayHi currAnimal = (SayHi) constructorStr.newInstance();
			System.out.println("current animal goes: " + currAnimal.makeSound());
		}
	}
}