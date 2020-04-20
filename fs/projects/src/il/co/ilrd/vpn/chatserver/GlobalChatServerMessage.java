package il.co.ilrd.vpn.chatserver;

import java.io.Serializable;


class GlobalChatServerMessage implements GlobalMessage<GlobalChatProtocolKeys, String> , Serializable{
	private static final long serialVersionUID = 1L;
	private GlobalChatProtocolKeys key;
	private String data;
	
	public GlobalChatServerMessage (GlobalChatProtocolKeys key, String data) {
		this.key = key;
		this.data = data;
	}
	
	@Override
	public GlobalChatProtocolKeys getKey() {
		return key;
	}
	
	@Override
	public String getData() {
		return data;
	}
	
	@Override
	public String toString() {
		return null;
	}

	public void setKey(GlobalChatProtocolKeys key) {
		this.key = key;
	}

	public void setData(String data) {
		this.data = data;
	}	
}
