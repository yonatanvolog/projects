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
	private final static String CLASS_SUFFIX  = ".class";
	private final static String SLASH  = "/";
	private final static String DOT  = ".";
	private final static String FILE_PREFIX = "file://";
	private static Map<String, Integer> currVersionsMap = new HashMap<>();

	public static List<Class<?>> load(String interfaceName, String jarPath) throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
		List<Class<?>> classesThatImplementInterface = new LinkedList<>();
		JarFile jarFile = new JarFile(new File(jarPath));
		Enumeration<?> jarEntries = jarFile.entries();
		URLClassLoader classLoader = new URLClassLoader(new URL[] {new URL(FILE_PREFIX + jarPath)});
		
		while(jarEntries.hasMoreElements()) {
			String iterEntry = jarEntries.nextElement().toString();
			if (iterEntry.contains(CLASS_SUFFIX)) {
				iterEntry = convertJarEntryToClass(iterEntry);
				Class<?> iterClass = classLoader.loadClass(iterEntry);
				for (Class<?> implementedInterface : iterClass.getInterfaces()) {
					if(implementedInterface.getName().contains(interfaceName) && isVersionHigherThanCurrent(iterClass)) {
						classesThatImplementInterface.add(iterClass);
						System.out.println("JarLoader just loaded: " + iterClass);
						break;
					}
				}
			}
		}
		classLoader.close();
		jarFile.close();
		
		return classesThatImplementInterface;
	}
	
	
	private static String convertJarEntryToClass(String iterEntry) {
		iterEntry = iterEntry.replace(SLASH, DOT);
		iterEntry = iterEntry.replace(CLASS_SUFFIX, EMPTY_STRING);
		
		return iterEntry;
	}
	
	private static boolean isVersionHigherThanCurrent(Class<?> iterClass) throws InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {		
		Method method = iterClass.getDeclaredMethod("getVersion");
		Object instance = iterClass.getDeclaredConstructor().newInstance();
		Integer iterClassVersion = (Integer)method.invoke(instance);
		
		Integer currClassVersion = currVersionsMap.get(iterClass.getName());
		if(null == currClassVersion) {
			currClassVersion = -1;
		}
		
		if(iterClassVersion > currClassVersion) {
			System.out.println("current version of " + iterClass + " is: " + currClassVersion + " ,new version is: " + iterClassVersion);
			currVersionsMap.put(iterClass.getName(), iterClassVersion);
				
			return true;
		}
		
		return false;
	}
}