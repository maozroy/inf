package il.co.ilrd.jarloader;

import java.awt.List;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.LinkedList;

public class test {

	public static void main(String[] args) throws ClassNotFoundException, IOException, IllegalAccessException, IllegalArgumentException, InvocationTargetException {
		java.util.List<Class<?>> list = JarLoader.load("RunMe", "/home/student/git/maoz-roytman/fs/testingjar/bin/testjar");
		System.out.println(list.size());
		for (Class<?> class1 : list) {
			Method[] methods = class1.getMethods();
		
			for (Method method : methods) {
				System.out.println(method.getName());
//				if (method.getName().equals("RunMeFirst")) {
//					System.out.println(method.invoke(new Object[] {null}));
//				}
			}
		}

	}

}
