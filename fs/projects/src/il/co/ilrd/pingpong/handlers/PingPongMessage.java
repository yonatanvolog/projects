package il.co.ilrd.pingpong.handlers;

import java.io.Serializable;

public class PingPongMessage implements Message<Integer, Message<String, Void>> , Serializable {
	
	private static final long serialVersionUID = 1L;
	private Integer key;
	private Message<String, Void> data;
	
	public PingPongMessage(Integer key, String messageKey) {
		this.key = key;
		data = new InnerMessage(messageKey);
	}
	
	@Override
	public Integer getKey() {
		return key;
	}

	@Override
	public Message<String, Void> getData() {
		return data;
	}
	
	private class InnerMessage implements Message<String, Void>  , Serializable {
		
		private static final long serialVersionUID = 1L;
		private String key;
		private Void data;
		
		public InnerMessage(String key) {
			this.key = key;
		}
		
		@Override
		public String getKey() {
			return key;
		}

		@Override
		public Void getData() {
			return data;
		}	
	}
}
