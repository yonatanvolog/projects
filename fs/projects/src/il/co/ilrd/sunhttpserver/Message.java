package il.co.ilrd.sunhttpserver;

public interface Message<K, D> {
	public K getKey(); //protocol flag
	public D getData();
}