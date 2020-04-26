package il.co.ilrd.databasemanagementserver;

import java.io.Serializable;

public class ServerMessage implements Message<ProtocolType, Message<?, ?>> , Serializable {
	
	private static final long serialVersionUID = 1L;
	private ProtocolType key;
	private Message<?, ?> data;
	
	public ServerMessage(ProtocolType key, Message<?, ?> data) {
		this.key = key;
		this.data = data;
	}

	@Override
	public ProtocolType getKey() {
		return key;
	}

	@Override
	public Message<?, ?> getData() {
		return data;
	}
}