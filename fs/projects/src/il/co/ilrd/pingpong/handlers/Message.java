package il.co.ilrd.pingpong.handlers;

public interface Message<K, D> {
	public K getKey(); //protocol flag
	public D getData();
}