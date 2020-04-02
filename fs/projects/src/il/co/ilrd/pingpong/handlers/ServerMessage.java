package il.co.ilrd.pingpong.handlers;

import java.io.Serializable;

public class ServerMessage<K, D> implements Message<ProtocolIndex, Message<K, D>> , Serializable {
	
	private static final long serialVersionUID = 1L;
	private ProtocolIndex key;
	private Message<K, D> data;
	
	public ServerMessage(ProtocolIndex key, Message<K, D> data) {
		this.key = key;
		this.data = data;
	}

	@Override
	public ProtocolIndex getKey() {
		return key;
	}

	@Override
	public Message<K, D> getData() {
		return data;
	}
}
