package il.co.ilrd.vpn.chatserver;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

public class RunClientTcpPing implements Runnable{
	String message;
	int bufferSize;
	int portNum;
	int sleep;
	boolean toContinue;
	
	public static final int BUFFER_SIZE = 1024;

	public static void main(String[] args) {
		new Thread(new RunClientTcpPing("Ping1", GlobalProtocolPort.PINGPONG_PROTOCOL_PORT.getPort(), BUFFER_SIZE, 1500)).start();
	}
	
	public RunClientTcpPing(String message, int portNum, int bufferSize, int sleep) {
		this.message = message;
		this.bufferSize = bufferSize;
		this.portNum = portNum;
		this.sleep = sleep;
		toContinue = true;
	}

	@Override
	public void run() {
		try {
			GlobalServerMessage messageToSend = new GlobalServerMessage(GlobalProtocolType.PINGPONG, new PingPongMessage(message));
			ByteBuffer buffer = ByteBuffer.allocate(bufferSize); 
			
			System.err.println("tryina connect");

		//	SocketChannel client = SocketChannel.open(new InetSocketAddress(InetAddress.getLocalHost(), portNum));
			//SocketChannel client = SocketChannel.open(new InetSocketAddress("172.20.20.13", portNum));
			
			SocketChannel client = SocketChannel.open(new InetSocketAddress("172.20.20.11", GlobalProtocolPort.PINGPONG_PROTOCOL_PORT.getPort()));

			System.err.println("connected!");

			
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

