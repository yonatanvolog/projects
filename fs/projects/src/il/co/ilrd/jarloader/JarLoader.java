package il.co.ilrd.jarloader;

import java.io.File;
import java.io.IOException;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.JarFile;

public class JarLoader {
	private final String EMPTY_STRING = "";
	private final String CLASS_SUFFIX  = ".class";

	public List<Class<?>> load(String interfaceName, String jarPath) throws IOException, ClassNotFoundException {
		List<Class<?>> classesThatImplementInterface = new LinkedList<>();
		JarFile jarFile = new JarFile(new File(jarPath));
		
		for(Enumeration<?> jarEntries = jarFile.entries(); jarEntries.hasMoreElements();) {
			String currEntry = jarEntries.nextElement().toString();
			
			if (currEntry.contains(CLASS_SUFFIX)) {	
				currEntry = currEntry.replace("/", ".");
				currEntry = currEntry.replace(CLASS_SUFFIX, EMPTY_STRING);
				Class<?> currEntryAsClass = Class.forName(currEntry);
				Class<?>[] interfacesImplmentedBycurrEntry = currEntryAsClass.getInterfaces();
				for (Class<?> implementedInterface : interfacesImplmentedBycurrEntry) {
					if(implementedInterface.getName().contains(interfaceName)) {
						classesThatImplementInterface.add(currEntryAsClass);
					}
				}
			}
		}
		jarFile.close();
		
		return classesThatImplementInterface;
	}
}