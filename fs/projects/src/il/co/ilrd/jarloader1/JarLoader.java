package il.co.ilrd.jarloader1;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.jar.JarFile;

public class JarLoader {
	private final static String EMPTY_STRING = "";
	private final static String CLASS_EXTENSION  = ".class";
	private final static String SLASH  = "/";
	private final static String DOT  = ".";
	private final static String FILE_PREFIX = "file://";

	public static List<Class<?>> load(String interfaceName, String jarPath) throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
		List<Class<?>> classesThatImplementInterface = new LinkedList<>();
		JarFile jarFile = new JarFile(new File(jarPath));
		Enumeration<?> jarEntries = jarFile.entries();
		URLClassLoader classLoader = new URLClassLoader(new URL[] {new URL(FILE_PREFIX + jarPath)});
		
		while(jarEntries.hasMoreElements()) {
			String iterEntry = jarEntries.nextElement().toString();
			if (iterEntry.contains(CLASS_EXTENSION)) {
				iterEntry = convertJarEntryToClass(iterEntry);
				Class<?> iterClass = classLoader.loadClass(iterEntry);
				for (Class<?> implementedInterface : iterClass.getInterfaces()) {
					if(isImplementInterface(implementedInterface, interfaceName)) {
						classesThatImplementInterface.add(iterClass);
					}
				}
			}
		}
		classLoader.close();
		jarFile.close();
		
		return classesThatImplementInterface;
	}
	
	private static boolean isImplementInterface(Class<?> class1 , String interfaceName) {
		return class1.getName().contains(interfaceName);
	}
	
	private static String convertJarEntryToClass(String iterEntry) {
		iterEntry = iterEntry.replace(SLASH, DOT);
		iterEntry = iterEntry.replace(CLASS_EXTENSION, EMPTY_STRING);
		
		return iterEntry;
	}
}