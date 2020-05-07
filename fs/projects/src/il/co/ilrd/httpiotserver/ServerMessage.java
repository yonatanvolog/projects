package il.co.ilrd.httpiotserver;

import java.io.Serializable;


public class ServerMessage implements Message<ProtocolType, Message<?, ?>> ,Serializable{
		
	private static final long serialVersionUID = 55555;
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


	public void setData(Message<?, ?> data) {
		this.data = data;
	}

	@Override
	public Message<?, ?> getData() {
		// TODO Auto-generated method stub
		return data;
	}
}