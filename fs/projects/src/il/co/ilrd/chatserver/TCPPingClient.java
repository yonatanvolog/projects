package il.co.ilrd.chatserver;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

public class TCPPingClient implements Runnable{
	String message;
	int bufferSize;
	int portNum;
	int sleep;
	boolean toContinue;
	
	public TCPPingClient(String message, int portNum, int bufferSize, int sleep) {
		this.message = message;
		this.bufferSize = bufferSize;
		this.portNum = portNum;
		this.sleep = sleep;
		toContinue = true;
	}

	@Override
	public void run() {
		try {
			ServerMessage messageToSend = new ServerMessage(ProtocolType.PINGPONG, new PingPongMessage(message));
			ByteBuffer buffer = ByteBuffer.allocate(bufferSize); 
			SocketChannel client = SocketChannel.open(new InetSocketAddress(InetAddress.getLocalHost(), portNum));
			new ClientInputScanner().start();
			
			while(toContinue) {  
		    	byte[] array1 = BytesUtil.toByteArray(messageToSend);
		    	buffer = ByteBuffer.wrap(array1);
				client.write(buffer);
				buffer.flip();
				client.read(buffer);
				String msg = (String) BytesUtil.toObject(buffer.array());
				System.out.println("TCP client " + message + " recieved: " + msg);
				Thread.sleep(sleep);     
			}
			System.out.println("TCP client " + message + " terminated");

		} catch (Exception e) {
			System.err.println("TCP client " + message + " crashed");
		}
	}
	
	private class ClientInputScanner extends Thread {
    	@Override
    	public void run() {
    		System.out.println("Awaiting user input");
    		Scanner scan = new Scanner(System.in);
    		String myLine = null;
    		while(toContinue) {
    			myLine = scan.nextLine();
    			if(myLine.contentEquals("exit")) {
    				toContinue = false;
    			}	
    		}
    		scan.close();
    	}
    }
}

