package il.co.ilrd.httpiotserver;

public interface Message<K, D> {
	public K getKey(); //protocol flag
	public D getData();
}