package il.co.ilrd.jarloader1;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.List;

public class TestJarLoader {

	public static void main(String[] args) throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {	
		String interfaceName = "FactoryCommandModifier";
		String jarPath = "/home/yonatan/Yonatan-Vologdin/fs/projects/bin/jars/CompanyRegistrationCommand.jar";
		
		List<Class<?>> loader = JarLoader.load(interfaceName, jarPath);
		loader.forEach(System.out::println); //magic
		
		System.out.println("These are the classes that impement interface - " + interfaceName + ":");
		for (Class<?> class1 : loader) {
			Method method = class1.getDeclaredMethod("getVersion");
			System.out.println("loader moethod is: " + method);
			Object instance = class1.getDeclaredConstructor().newInstance();
			Integer version = (Integer)method.invoke(instance);
			System.out.println("The version is:" + version);
		}
	}
}