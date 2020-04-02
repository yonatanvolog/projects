package il.co.ilrd.pingpong.handlers;

public class RunClients {
	public static final int TCP_PORT1 = 45000;
	public static final int TCP_PORT2 = 50000;
	public static final int UDP_PORT1 = TCP_PORT1;
	public static final int UDP_PORT2 = TCP_PORT2;
	public static final int BROADCAST_PORT1 = 60000;
	public static final int BROADCAST_PORT2 = 65000;
	
	public static final int BUFFER_SIZE = 1024;

	public static void main(String[] args) {
		new Thread(new TCPClient("Ping1", 45000, BUFFER_SIZE, 1500)).start();
		new Thread(new TCPClient("Ping2", 50000, BUFFER_SIZE, 2000)).start();
		
		new Thread(new UDPClient("Ping1", 45000, BUFFER_SIZE, 1500)).start();
		new Thread(new UDPClient("Ping2", 50000, BUFFER_SIZE, 1500)).start();
		
		new Thread(new BroadcastClient("Ping1", 60000, BUFFER_SIZE, 1500)).start();
		new Thread(new BroadcastClient("Ping2", 65000, BUFFER_SIZE, 1500)).start();
	}
}
