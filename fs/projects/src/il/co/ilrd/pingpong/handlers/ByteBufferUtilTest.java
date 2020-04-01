package il.co.ilrd.pingpong.handlers;

import java.io.IOException;
import java.nio.ByteBuffer;

public class ByteBufferUtilTest {

	public static void main(String[] args) throws ClassNotFoundException, IOException {
		PingPongMessage origMessage = new PingPongMessage(1, "message");
		ByteBuffer buffer = BufferArrayUtil.toByteBuffer(origMessage);
		System.out.println("mid test");
		PingPongMessage newMessage = (PingPongMessage)BufferArrayUtil.toMessage(buffer);
		System.out.println(newMessage.getKey() + " " + newMessage.getData().getKey());
	}

}
