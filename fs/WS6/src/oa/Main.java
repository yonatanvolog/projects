package oa;

import java.lang.reflect.*;

public class Main {

	public static void main(String[] args) 
			throws InstantiationException, IllegalAccessException 
	{
	
		Class fooClass = Foo.class;	
		System.out.println("Foo ancestor: " + fooClass.getSuperclass().getName());
		
		int modifiers = fooClass.getModifiers();
		System.out.println("Foo Modifiers: " + Modifier.toString(modifiers));
		
	    Class fooInterfaces[] = fooClass.getInterfaces();
	    for (Class inter : fooInterfaces)  
	    { 
	        System.out.println(inter);
	    }
	    
	    Field[] fields = fooClass.getDeclaredFields();
	    for (Field field : fields)  
	    { 
	        System.out.println(field);
	    }
	    
	    Method[] methods = fooClass.getMethods();
	    for (Method method : methods)  
	    { 
	        System.out.println(method);
	    }
	    
        Object obj = fooClass.newInstance();
        Foo fooInstFoo = (Foo)obj;
        fooInstFoo.sayhi();
	}
}
