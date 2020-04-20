package il.co.ilrd.vpn.chatserver;

public interface GlobalMessage<K, D> {
	
	public K getKey();
	public D getData();	
}
