package il.co.ilrd.selector.pingpongbroadcast;

import static il.co.ilrd.selector.pingpongbroadcast.TcpUdpPongServer.*;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.Scanner;

public class UdpPingClientByteBuffer2 {
    private static DatagramChannel client;
    private static ByteBuffer buffer; 
    private static boolean toContinue = true;

	public static void main(String[] args) throws IOException, InterruptedException {
		client = DatagramChannel.open();
		client.bind(null);
		buffer = ByteBuffer.allocate(BUFFER_SIZE);
		new UdpPingClientByteBuffer2().new ClientInputScanner().start();
		
		while(toContinue) {
        	Thread.sleep(1500);
			String response = sendMessage("Ping2");
			System.out.println("Client received: " + response);
		}
		client.close();
		System.out.println("Client terminated");
    }
 
    private static String sendMessage(String msg) throws IOException {
		buffer = ByteBuffer.wrap(msg.getBytes());
        SocketAddress serverAddress = new InetSocketAddress(UDP_PORT);
		client.send(buffer ,serverAddress);
		buffer.flip();
		buffer.clear();
		client.receive(buffer);
   		String serverMessage = new String(buffer.array()).trim();

   		return serverMessage;
    }
    
    private class ClientInputScanner extends Thread {
    	@Override
    	public void run() {
    		System.out.println("Awaiting user input");
    		Scanner scan = new Scanner(System.in);
    		String myLine = scan.nextLine();
    		while(toContinue) {
    			if(myLine.contentEquals("exit")) {
    				toContinue = false;
    			}	
    		}
    		scan.close();
    	}
    }
}