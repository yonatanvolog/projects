package il.co.ilrd.observer;

public class Main {

	public static void main(String[] args) {
		Subject s = new Subject();
		Observer<Integer> ob = new Observer();
		ob.register(s);
		
		s.updateAll();
	}

}
