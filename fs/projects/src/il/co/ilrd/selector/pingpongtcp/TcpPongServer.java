package il.co.ilrd.selector.pingpongtcp;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Scanner;
import java.util.Set;

public class TcpPongServer {
    public static final int TCP_PORT = 55000;
    public static final int BUFFER_SIZE = 256;

    private static final String EXIT_MESSAGE = "exit";
    private static boolean toContinue = true;

    public static void main(String[] args) throws IOException {
    	
        Selector selector = Selector.open();
        ServerSocketChannel tcpServerSocket = ServerSocketChannel.open();
        tcpServerSocket.bind(new InetSocketAddress(InetAddress.getLocalHost(), TCP_PORT)); //what host?
        tcpServerSocket.configureBlocking(false);
        tcpServerSocket.register(selector, SelectionKey.OP_ACCEPT);
        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
 
        new TcpPongServer() .new ServerInputScanner(tcpServerSocket).start();
        System.out.println("Server started running");
        
        while (toContinue) {
            selector.select();
            System.out.println("after selector.select();");

            Set<SelectionKey> selectedKeys = selector.selectedKeys();
            Iterator<SelectionKey> iter = selectedKeys.iterator();
            while (toContinue & iter.hasNext()) {
                System.out.println("in iter while");

                SelectionKey key = iter.next();
 
                if (key.isAcceptable()) {                	
                	register(selector, tcpServerSocket);
                }
 
                if (key.isReadable()) {
        			try {
						handleClient(buffer, key);
					} catch (IOException e) {
						key.cancel();
						System.out.println("Client closed socket (exited)");
					}
                }
                iter.remove();
            }
        }
		System.out.println("Server terminated");
    }
 
    private static void handleClient(ByteBuffer buffer, SelectionKey key)
      throws IOException {
  
        SocketChannel client = (SocketChannel) key.channel();
        client.read(buffer);
        String clientMessage = new String(buffer.array()).trim();
        System.out.println("Server received: " + clientMessage);
        String serverResponse = PingPingProtocol(clientMessage);
        buffer.flip();
        buffer = ByteBuffer.wrap(serverResponse.getBytes());
        System.err.println("server response: " + serverResponse);
        client.write(buffer);
        buffer.clear();
    }
    
    private static String PingPingProtocol(String clientMessage) {
	    if(clientMessage.equals("Pong1") | clientMessage.equals("Pong2")) {
	    	return "Ping";
	    } else if (clientMessage.equals("Ping1") | clientMessage.equals("Ping2")) {
	    	return "Pong";
	    } else {
	    	return "Wrong";
	    }
    }
    
    private class ServerInputScanner extends Thread {
    	private ServerSocketChannel tcpServerSocket;
    	
    	public ServerInputScanner(ServerSocketChannel tcpServerSocket) {
    		this.tcpServerSocket = tcpServerSocket;
    	}
    	
    	@Override
    	public void run() {
    		System.out.println("Awaiting user input");
    		Scanner scan = new Scanner(System.in);
    		String myLine = scan.nextLine();
    		if(myLine.contentEquals(EXIT_MESSAGE)) {
    			try {
    				toContinue = false;
					tcpServerSocket.close();
	                System.out.println("Not accepting client messages anymore");
				} catch (IOException e) {
					// couldn't close tcpServerSocket, cannot handle
				}
    	    }
    		scan.close();
    	}
    }
 
    private static void register(Selector selector, ServerSocketChannel serverSocket)
      throws IOException {
  
        SocketChannel client = serverSocket.accept();
        client.configureBlocking(false);
        client.register(selector, SelectionKey.OP_READ);
    }
}