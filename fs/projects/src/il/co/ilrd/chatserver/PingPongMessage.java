package il.co.ilrd.chatserver;

import java.io.Serializable;

public class PingPongMessage implements Message<String, Void> , Serializable {
	
	private static final long serialVersionUID = 1L;
	private String messageKey;

	public PingPongMessage(String messageKey) {
		this.messageKey = messageKey;
	}

	@Override
	public String getKey() {
		return messageKey;
	}

	@Override
	public Void getData() {
		return null;
	}
}
