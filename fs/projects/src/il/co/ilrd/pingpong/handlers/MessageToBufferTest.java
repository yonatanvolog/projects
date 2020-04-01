package il.co.ilrd.pingpong.handlers;

import java.io.IOException;

public class MessageToBufferTest {

	public static void main(String[] args) throws IOException, ClassNotFoundException {
		PingPongMessage origMessage = new PingPongMessage(1, "message");
		byte[] buffer = BytesUtil.toByteArray(origMessage);
		PingPongMessage newMessage = (PingPongMessage)BytesUtil.toObject(buffer);
		System.out.println(newMessage.getKey() + " " + newMessage.getData().getKey());
	}
}
