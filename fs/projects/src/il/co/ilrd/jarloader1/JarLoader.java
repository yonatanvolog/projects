package il.co.ilrd.jarloader1;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.JarFile;

public class JarLoader {
	private final static String EMPTY_STRING = "";
	private final static String CLASS_SUFFIX  = ".class";
	private final static String SLASH  = "/";
	private final static String DOT  = ".";
	private final static String FILE_PREFIX = "file://";

	public static List<Class<?>> load(String interfaceName, String jarPath) throws IOException, ClassNotFoundException {
		List<Class<?>> classesThatImplementInterface = new LinkedList<>();
		JarFile jarFile = new JarFile(new File(jarPath));
		Enumeration<?> jarEntries = jarFile.entries();
		URLClassLoader classLoader = new URLClassLoader(new URL[] {new URL(FILE_PREFIX + jarPath)});
		
		while(jarEntries.hasMoreElements()) {
			String currEntry = jarEntries.nextElement().toString();
			
			if (currEntry.contains(CLASS_SUFFIX)) {	
				currEntry = currEntry.replace(SLASH, DOT);
				currEntry = currEntry.replace(CLASS_SUFFIX, EMPTY_STRING);
				Class<?> currClass = classLoader.loadClass(currEntry);
				Class<?>[] interfacesImplmentedBycurrEntry = currClass.getInterfaces();
				for (Class<?> implementedInterface : interfacesImplmentedBycurrEntry) {
					if(implementedInterface.getName().contains(interfaceName)) {
						classesThatImplementInterface.add(currClass);
						System.out.println("just loaded: " + currClass);
						break;
					}
				}
			}
		}
		classLoader.close();
		jarFile.close();
		
		return classesThatImplementInterface;
	}
}