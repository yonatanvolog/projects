package il.co.ilrd.selector.pingpongtcp;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

import static il.co.ilrd.selector.pingpongtcp.TcpPongServer.*;

public class TcpPingClientByteBuffer2 {
    private static SocketChannel client;
    private static ByteBuffer buffer; 
    private static boolean toContinue = true;

	public static void main(String[] args) throws IOException, InterruptedException {
		client = SocketChannel.open(new InetSocketAddress(InetAddress.getLocalHost(), TCP_PORT));
		buffer = ByteBuffer.allocate(BUFFER_SIZE);
		
		new TcpPingClientByteBuffer2().new ClientInputScanner().start();
		
		while(toContinue) {
        	Thread.sleep(1500);
			String response = sendMessage("Ping2");
			System.out.println("Client received: " + response);
		}
		client.close();
		System.out.println("Client terminated");
    }
 
    private static String sendMessage(String msg) throws InterruptedException, IOException {
    	buffer = ByteBuffer.wrap(msg.getBytes());
    	String response = null;
        client.write(buffer);
		buffer.clear();
        client.read(buffer);
        response = new String(buffer.array()).trim();            
        buffer.clear();
        
        return response;
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
