package il.co.ilrd.selector.pingpongudptcp;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.Scanner;

import static il.co.ilrd.selector.pingpongudptcp.TcpUdpPongServer.*;

public class BroadcastPingClientByteBuffer1 {
    private static DatagramChannel client;
    private static ByteBuffer buffer; 
    private static boolean toContinue = true;

	public static void main(String[] args) throws IOException, InterruptedException {
		client = DatagramChannel.open();
        client.setOption(StandardSocketOptions.SO_BROADCAST, true);
		client.bind(null);
		buffer = ByteBuffer.allocate(BUFFER_SIZE);
		new BroadcastPingClientByteBuffer1().new ClientInputScanner().start();
		
		while(toContinue) {
        	Thread.sleep(3000);
			String response = sendMessage("Ping1");
			System.out.println("Client received: " + response);
		}
		client.close();
		System.out.println("Client terminated");
    }
 
    private static String sendMessage(String msg) throws IOException {
		buffer = ByteBuffer.wrap(msg.getBytes());
		InetAddress localHostAddress = InetAddress.getByName("255.255.255.255");
        SocketAddress serverAddress = new InetSocketAddress(localHostAddress, BROADCAST_PORT);
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