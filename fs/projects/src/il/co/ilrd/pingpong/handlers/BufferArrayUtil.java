package il.co.ilrd.pingpong.handlers;

import java.io.IOException;
import java.nio.ByteBuffer;

public class BufferArrayUtil {
    public static ByteBuffer toByteBuffer(Message<Integer, Message<String, Void>> message) throws IOException {
    	
    	byte[] buff = BytesUtil.toByteArray(message);
        return ByteBuffer.wrap(buff);
    }
    
    public static Message<Integer, Message<String, Void>> toMessage(ByteBuffer buffer) throws ClassNotFoundException, IOException {
    	//ByteBuffer buf = ByteBuffer.allocate(256);
    	byte[] arr = new byte[buffer.remaining()];
    	buffer.get(arr);
    	
    	return (Message<Integer, Message<String, Void>>) BytesUtil.toObject(arr);

    }
}
