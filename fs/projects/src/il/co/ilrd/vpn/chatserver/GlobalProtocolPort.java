package il.co.ilrd.vpn.chatserver;

public enum GlobalProtocolPort {
	PINGPONG_PROTOCOL_PORT(50000),
	CHAT_PROTOCOL_PORT(55555);
	
	private final int PORT;
	
	GlobalProtocolPort(int port) {
		this.PORT = port;
	}
	
	public int getPort() {
		return PORT;
	}
}