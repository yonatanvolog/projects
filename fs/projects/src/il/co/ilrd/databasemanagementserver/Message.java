package il.co.ilrd.databasemanagementserver;

public interface Message<K, D> {
	public K getKey(); //protocol flag
	public D getData();
}