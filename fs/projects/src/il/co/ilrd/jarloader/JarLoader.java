package il.co.ilrd.jarloader;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class JarLoader {
	private static JarFile jarFile;
	private final static String DOT = ".";
	private final static String BACKSLASH = "/";
	private final static String DOT_CLASS = ".class";
	private final static String FILE_PREFIX = "file://";
	
	public static List<Class<?>> load(String interfaceName, String jarPath) throws ClassNotFoundException, IOException {
		jarFile = new JarFile(new File(jarPath));
		List<Class<?>> listToReturn = new LinkedList<>();
		Enumeration<JarEntry> Entrylist = jarFile.entries();
		try(URLClassLoader classLoader = new URLClassLoader(new URL[] {new URL(FILE_PREFIX + jarPath)});){
			 while(Entrylist.hasMoreElements()) {
			        JarEntry entry = Entrylist.nextElement();
			        
					if(entry.getName().endsWith(DOT_CLASS)) {
						String className = entry.getName().substring(0, entry.getName().indexOf(DOT)).replace(BACKSLASH, DOT);
						Class<?> currentClass = classLoader.loadClass(className);
						for (Class<?> classIter : currentClass.getInterfaces()) {
							if (classIter.getName().contains(interfaceName)) {
								listToReturn.add(currentClass);
							}
						}					
					}
					
				}
		}
		return listToReturn;
	}
}