package il.co.ilrd.vpn.chatserver;

import java.io.Serializable;


public class GlobalServerMessage implements GlobalMessage<GlobalProtocolType, GlobalMessage<?, ?>> ,Serializable{
		
	private static final long serialVersionUID = 55555;
	private GlobalProtocolType key; 
	private GlobalMessage<?, ?> data;
		
	public GlobalServerMessage(GlobalProtocolType key, GlobalMessage<?, ?> data) {
		this.key = key;
		this.data = data;
	}


	@Override
	public GlobalProtocolType getKey() {
		return key;
	}


	public void setData(GlobalMessage<?, ?> data) {
		this.data = data;
	}

	@Override
	public GlobalMessage<?, ?> getData() {
		// TODO Auto-generated method stub
		return data;
	}
}