package il.co.ilrd.selector.pingpongbroadcast;

import java.io.Closeable;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.Channel;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

public class TcpUdpBroadcastPongServer {
	public static final int MAIN_PORT = 55000;
    public static final int TCP_PORT = MAIN_PORT;
    public static final int UDP_PORT = MAIN_PORT;
    public static final int BROADCAST_PORT = MAIN_PORT;
    public static final int BUFFER_SIZE = 256;
    
    private static final String EXIT_MESSAGE = "exit";
    private static boolean toContinue = true;
    
    public static void main(String[] args) throws IOException, InterruptedException {
    	Selector selector = Selector.open();
    
        ServerSocketChannel tcpServerSocket = ServerSocketChannel.open();
        DatagramChannel udpServerSocket = DatagramChannel.open();
      	DatagramChannel udpBroadcastServerSocket = DatagramChannel.open();
        
        configureTcpServerSocket(tcpServerSocket, selector, TCP_PORT);
        configureUdpServerSocket(udpServerSocket, selector, UDP_PORT);
        configureBroadcastServerSocket(udpBroadcastServerSocket, selector, BROADCAST_PORT);
        
        List<Closeable> clientSocketsList = new ArrayList<>();
        List<Closeable> serverSocketsList = new ArrayList<>();
        serverSocketsList.add(tcpServerSocket);
        serverSocketsList.add(udpServerSocket);
        serverSocketsList.add(udpBroadcastServerSocket);
        
        new TcpUdpBroadcastPongServer() .new ServerInputScanner(serverSocketsList).start();
        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
   
        System.out.println("Server started running");
        
        while (toContinue) {
            while(0 == selector.select(3000)) {}
            Set<SelectionKey> selectedKeys = selector.selectedKeys();
            Iterator<SelectionKey> iter = selectedKeys.iterator();
            while (toContinue & iter.hasNext()) {
                SelectionKey key = iter.next();
                Channel currentChannel = key.channel();
 
                if (key.isValid() && key.isAcceptable() && currentChannel == tcpServerSocket) {
                    clientSocketsList.add(register(selector, tcpServerSocket));
                }
                
                if (key.isValid() && clientSocketsList.contains(currentChannel)) {
                	try {
                		handleTcpClient(buffer, key);
                	} catch (IOException e) {
                		key.cancel();
                		System.out.println("TCP Client closed socket (exited)");
                	}
                }
                
                if (key.isValid() && key.isReadable()) {
                	if(key.isValid() && currentChannel == udpServerSocket) {
                		try {
    						handleUdpClient(buffer, key);
            			} catch (IOException e) {
    						key.cancel();
    						System.out.println("UDP Client closed socket (exited)");
    					}
                	}
                	
                	if(key.isValid() && currentChannel == udpBroadcastServerSocket) {
                		try {
            				handleUdpBroadcastClient(buffer, key, udpServerSocket);
            			} catch (IOException e) {
    						key.cancel();
    						System.out.println("Broadcast Client closed socket (exited)");
    					} 
                	}
                }
                iter.remove();
            }
        }
		System.out.println("Server terminated");
    }
 
    private static void configureTcpServerSocket(ServerSocketChannel serverSocket, Selector selector, int port) throws IOException {
    	serverSocket.bind(new InetSocketAddress(InetAddress.getLocalHost(), port));
    	serverSocket.configureBlocking(false);
    	serverSocket.register(selector, SelectionKey.OP_ACCEPT);
    }
    
    private static void configureUdpServerSocket(DatagramChannel serverSocket, Selector selector, int port) throws IOException {
    	serverSocket.socket().bind(new InetSocketAddress(InetAddress.getLocalHost(),port));
    	serverSocket.configureBlocking(false);
    	serverSocket.register(selector, SelectionKey.OP_READ);
    }
    
    private static void configureBroadcastServerSocket(DatagramChannel serverSocket, Selector selector, int port) throws IOException {
    	serverSocket.socket().bind(new InetSocketAddress(InetAddress.getByName("255.255.255.255"),port));
    	serverSocket.setOption(StandardSocketOptions.SO_BROADCAST, true);
    	serverSocket.configureBlocking(false);
    	serverSocket.register(selector, SelectionKey.OP_READ);
    }

    private static void handleTcpClient(ByteBuffer buffer, SelectionKey key)
    	      throws IOException {
    	        SocketChannel client = (SocketChannel) key.channel();
    	        if(-1 == client.read(buffer)) {
    	        	throw new IOException();
    	        }
    	        String clientMessage = new String(buffer.array()).trim();
    	        System.out.println("Server received from TCP client: " + clientMessage);
    	        buffer.flip();
    	        
    	        String serverResponse = PingPingProtocol(clientMessage);
    	        buffer = ByteBuffer.wrap(serverResponse.getBytes());
    	        while(buffer.hasRemaining()) {
    	        	client.write(buffer);
                }
    	        buffer.clear();
    	    }
    	    
    	    private static void handleUdpClient(ByteBuffer buffer, SelectionKey key) throws IOException {
    	    	DatagramChannel channel = (DatagramChannel) key.channel();   
    	    	SocketAddress clientAddress = null;
    	    	while(null == clientAddress) {
    	    		clientAddress = channel.receive(buffer);
    	    	}
    			String clientMessage = new String(buffer.array()).trim();
    			System.out.println("Server received from UDP client: " + clientMessage);
    			String serverResponse = PingPingProtocol(clientMessage);
    			buffer.flip();
    			buffer = ByteBuffer.wrap(serverResponse.getBytes());
    			channel.send(buffer, clientAddress);

    			buffer.clear();
    	    }
    	    
    	    private static void handleUdpBroadcastClient(ByteBuffer buffer, SelectionKey key, DatagramChannel udpServerSocket) throws IOException {
    	    	DatagramChannel channel = (DatagramChannel) key.channel();
      	    	SocketAddress clientAddress = null;
    	    	while(null == clientAddress) {
    	    		clientAddress = channel.receive(buffer);
    	    	}
    	    	String clientMessage = new String(buffer.array()).trim();
    			System.out.println("Server received from Broadcast client: " + clientMessage);
    			String serverResponse = PingPingProtocol(clientMessage);
    			buffer.flip();
    			buffer = ByteBuffer.wrap(serverResponse.getBytes());
    			udpServerSocket.send(buffer, clientAddress);

    			buffer.clear();
    	    }
    	    
    	    private static SocketChannel register(Selector selector, ServerSocketChannel serverSocket)
    	      throws IOException {
    	  
    	        SocketChannel client = serverSocket.accept();
    	        client.configureBlocking(false);
    	        client.register(selector, SelectionKey.OP_READ);
    	        
    	        return client;
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
    	private List<Closeable> serverSocketsList;
    	
    	public ServerInputScanner(List<Closeable> serverSocketsList) {
    		this.serverSocketsList = serverSocketsList;
    	}
    	
    	@Override
    	public void run() {
    		System.out.println("Awaiting user input");
    		Scanner scan = new Scanner(System.in);
    		String myLine = scan.nextLine();
    		if(myLine.contentEquals(EXIT_MESSAGE)) {
    			try {
    				toContinue = false;
    				for (Closeable serverSocketIter : serverSocketsList) {
    					serverSocketIter.close();
					}
	                System.out.println("Not accepting client messages anymore");
				} catch (IOException e) {
					// couldn't close tcpServerSocket, cannot handle
				}
    	    }
    		scan.close();
    	}
    }
}