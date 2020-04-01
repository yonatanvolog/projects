package il.co.ilrd.pingpong.handlers;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.Scanner;

public class UdpPingClientMessage implements Runnable{
    private static DatagramChannel client;
    private static boolean toContinue = false;
    static int UDP_PORT = 50000;
    static int BUFFER_SIZE = 256;
    static ByteBuffer buffer;
    static int PING_PONG_PROTOCOL_INDEX = 0;
    String messageToSend;
    int portNumber = 50000;
	
	
    public UdpPingClientMessage(String messageToSend) {
    	toContinue = true;
    	this.messageToSend = messageToSend;
    }  
 
    private static String sendMessage(String msg) throws IOException, ClassNotFoundException {
    	
    	PingPongMessage message = new PingPongMessage(PING_PONG_PROTOCOL_INDEX, msg);
    	DatagramPacket packet = null;
    	
    	try (DatagramSocket socket = new DatagramSocket()){
    		byte[] buffer = BytesUtil.toByteArray(message);
    		packet = new DatagramPacket(buffer, buffer.length, InetAddress.getLocalHost(), UDP_PORT);
    		socket.send(packet);
    		socket.receive(packet);
    	}

   		return (String) BytesUtil.toObject(packet.getData());
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

	@Override
	public void run() {
		try {
			client = DatagramChannel.open();
			client.bind(null);
			buffer = ByteBuffer.allocate(BUFFER_SIZE);
			new UdpPingClientMessage(null).new ClientInputScanner().start();
			
			while(toContinue) {
	        	Thread.sleep(3000);
				String response = sendMessage(messageToSend);
				System.out.println("UDP Client received: " + response);
			}
			client.close();
			System.out.println("UDP Client terminated");
		} catch (Exception e) {
			System.err.println("UDP Client crashed");
		}
	}
}
