package il.co.ilrd.chatserver;

public class RunPingClients {
	public static final int BUFFER_SIZE = 1024;

	public static void main(String[] args) {
		//new Thread(new TCPPingClient("Ping1", 50000, BUFFER_SIZE, 1500)).start();
		
		new Thread(new UDPPingClient("Ping2", ProtocolPort.PINGPONG_PROTOCOL_PORT.getPort(), BUFFER_SIZE, 1500)).start();
	}
}
