//Run -> Run Configuration -> Arguments -> (enter) java.lang.Class -> Apply
public class ClassInfo {
	public static void main(String[] args) throws ClassNotFoundException {
		Class cls = Class.forName(args[0]);
		Class c  = Circle.class;
		
		System.out.println(cls.toString());
		System.out.println(cls.getName());
		System.out.println(cls.getSuperclass());
		System.out.println(cls.getClassLoader());
		System.out.println(cls.getFields());
		System.out.println(cls.getPackageName());
		System.out.println(cls.getPackage());
		
		System.out.println(c.toString());
		System.out.println(c.getName());
		System.out.println(c.getSuperclass());
		System.out.println(c.getClassLoader());
		System.out.println(c.getFields());
		System.out.println(c.getPackageName());
		System.out.println(c.getPackage());
	}
}
