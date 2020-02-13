package singleton_package;

//This is a slight optimisation to Singleton6, in this case the object is instantized 
//only at the point of "return object", and because the field inside SingletonHolder
//is static, we will return the same instance each time
		
public class Singleton7 {
	private Singleton7 (){}
		
	public static class SingletonHolder {
		public static Singleton7 instance = new Singleton7();	
	}
	public static Singleton7 getInstance() {
		return SingletonHolder.instance;
	}
}
