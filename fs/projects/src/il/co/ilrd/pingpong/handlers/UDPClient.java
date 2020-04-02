package il.co.ilrd.pingpong.handlers;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Scanner;

public class UDPClient implements Runnable{
	String message;
	int bufferSize;
	int portNum;
	int sleep;
	boolean toContinue;
	
	public UDPClient(String message, int portNum, int bufferSize, int sleep) {
		this.message = message;
		this.bufferSize = bufferSize;
		this.portNum = portNum;
		this.sleep = sleep;
		toContinue = true;
	}
	
	@Override
	public void run() {		
		DatagramPacket packet = null;
		ServerMessage<ProtocolIndex, PingPongMessage> messageToSend = new ServerMessage(ProtocolIndex.PingPong, new PingPongMessage(message));
		new ClientInputScanner().start();
		
		while(toContinue) {			
			try (DatagramSocket socket = new DatagramSocket()){
				byte[] buffer = BytesUtil.toByteArray(messageToSend);
				packet = new DatagramPacket(buffer, buffer.length, InetAddress.getLocalHost(), portNum);	
				socket.send(packet);
				socket.receive(packet);			
				String msg = (String) BytesUtil.toObject(packet.getData());
				System.out.println("UDP client " + message + " recieved: " + msg);
				Thread.sleep(sleep);
			} catch (Exception e) {
				System.err.println("UDP client " + message + " crashed");
			}
		}
		System.out.println("UDP client " + message + " terminated");
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

