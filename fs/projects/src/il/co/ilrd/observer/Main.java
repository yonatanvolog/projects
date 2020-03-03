package il.co.ilrd.observer;

public class Main {

	public static void main(String[] args) {
		Subject<Integer> s1 = new Subject<>();
		Subject<Integer> s2 = new Subject<>();

		Observer<Integer> ob = new Observer<>();
		ob.register(s1);
		s1.updateAll();
		s1.stopUpdate();
		
		ob.unregister(s1);
		
		ob.register(s2);
		ob.unregister();
	}
}
