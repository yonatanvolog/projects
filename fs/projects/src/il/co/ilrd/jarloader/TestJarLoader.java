package il.co.ilrd.jarloader;

import java.io.IOException;
import java.util.List;

public class TestJarLoader {

	public static void main(String[] args) throws IOException, ClassNotFoundException {
		String interfaceName = "SayHi";
		String jarPath = "/home/student/Yonatan-Vologdin/fs/projects/bin/AnimalSounds.jar";
		
		List<Class<?>> loader = new JarLoader().load(interfaceName, jarPath);
		
		System.out.println("These are the classes that impement interface - " + interfaceName + ":");
		for (Class<?> class1 : loader) {
			System.out.println(class1.toString());
		}
	}
}