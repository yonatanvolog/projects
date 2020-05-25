package il.co.ilrd.z_experiments;

public class main {

	public static void main(String[] args) {
		Dog dog = new Dog("yellow");
		System.err.println("dog color is:" + dog.color + dog.obj.toString());
		changeDogColor(dog);
		System.err.println("dog color is:" + dog.color + dog.obj.toString());
	}
	
	private static void changeDogColor(Dog dog) {
		dog = new Dog("brown");
		dog.color = "blue";
		dog.obj = new Object();
	}

}
