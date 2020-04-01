package il.co.ilrd.pingpong.handlers;

import java.io.Closeable;
import java.io.IOException;
import java.net.DatagramPacket;
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
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.print.DocFlavor.BYTE_ARRAY;

public class Server {
	public static final int MAIN_PORT = 50000;
    public static final int TCP_PORT = MAIN_PORT;
    public static final int UDP_PORT = MAIN_PORT;
    public static final int BROADCAST_PORT = MAIN_PORT;
    public static final int BUFFER_SIZE = 1024;
	
    //ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);

    
	private ConnectionHandler connectionHandler;
	private MessageHandler messageHandler;
	
	public Server() throws IOException {
		connectionHandler = new ConnectionHandler();
		messageHandler = new MessageHandler();
	}
	
	public void startServer() throws IOException, ClassNotFoundException {
		connectionHandler.startConnections();
	}

	public void stopServer() {
		connectionHandler.stopConnections();
		//connectionHandler.removeConnection()?
	}

	public void addBroadcastConnection(int portNumber) throws IOException {
		BroadcastConnection broadcastConnection = new BroadcastConnection(connectionHandler.selector, BROADCAST_PORT);
		connectionHandler.addConnection(broadcastConnection);
	}

	public void addTcpConnection(int portNumber) throws IOException {
		TcpConnection tcpConnection = new TcpConnection(connectionHandler.selector, TCP_PORT);	
		connectionHandler.addConnection(tcpConnection);
	}

	public void addUdpConnection(int portNumber) throws IOException {
		UdpConnection udpConnection = new UdpConnection(connectionHandler.selector, UDP_PORT);
		connectionHandler.addConnection(udpConnection);
	}	
	
	/**********************************************
	 * Connection Handler
	 **********************************************/
	private class ConnectionHandler {
		private List<Connection> serverConnetionsList; //why do we need a list of connections if we already registered to the selector?
    	private Selector selector;
		boolean toContinue;
		private Map<Channel, Connection> serverConnectionsMap = new HashMap<>();
		
		
		public ConnectionHandler() throws IOException {
			serverConnetionsList = new ArrayList<>();
			selector = Selector.open();
			toContinue = true;
		}
		
		private void startConnections() throws IOException, ClassNotFoundException {
	        List<Closeable> clientSocketsList = new ArrayList<>();
			
	        
	        for (Connection serverConnection : serverConnetionsList) {
	        	serverConnection.configureServerSocket(selector);
	        	serverConnectionsMap.put(serverConnection.getChannel(), serverConnection);
			}
	        
	        
			while (toContinue) {
	            while(0 == selector.select(3000)) {}
	            Set<SelectionKey> selectedKeys = selector.selectedKeys();
	            Iterator<SelectionKey> iter = selectedKeys.iterator();
	            while (toContinue & iter.hasNext()) {
	                SelectionKey key = iter.next();
	                Channel currentChannel = key.channel();
	 
              
	                
	                if(key.isValid() && key.isReadable()) {	//UDP server connection
	                    //ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
	                	
                		Connection currentConnection = serverConnectionsMap.get(currentChannel);
                		                		
                		
                		ByteBuffer buffer = currentConnection.receiveMessage();
                		messageHandler.handleMessage(buffer, currentConnection);
                	}
	                
//	                if (key.isValid() && key.isAcceptable() && currentChannel == tcpServerSocket) {
//	                    clientSocketsList.add(register(selector, tcpServerSocket));
//	                }
//	                
//	                if (key.isValid() && clientSocketsList.contains(currentChannel)) {
//	                	try {
//	                		handleTcpClient(buffer, key);
//	                	} catch (IOException e) {
//	                		key.cancel();
//	                		System.out.println("TCP Client closed socket (exited)");
//	                	}
//	                }
//	                
//	                if (key.isValid() && key.isReadable()) {
//	                	if(key.isValid() && currentChannel == udpServerSocket) {
//	                		try {
//	    						handleUdpClient(buffer, key);
//	            			} catch (IOException e) {
//	    						key.cancel();
//	    						System.out.println("UDP Client closed socket (exited)");
//	    					}
//	                	}
//	                	
//	                	if(key.isValid() && currentChannel == udpBroadcastServerSocket) {
//	                		try {
//	            				handleUdpBroadcastClient(buffer, key, udpServerSocket);
//	            			} catch (IOException e) {
//	    						key.cancel();
//	    						System.out.println("Broadcast Client closed socket (exited)");
//	    					} 
//	                	}
//	                }
	                iter.remove();
	            }
	        }
			System.out.println("Server terminated");
		}

		private void stopConnections() {
//			toContinue = false;
//			for (Closeable serverSocketIter : serverConnetionsList) {
//				serverSocketIter.close();
//			}
		}

		private void addConnection(Connection connection) {
			serverConnetionsList.add(connection);
		}

		private void removeConnection(Connection connection) {
			serverConnetionsList.remove(connection);
		}
	}
	
	/**********************************************
	 * Connection Interface
	 **********************************************/

	private interface Connection { //implements closeable 
		//public void sendMessage(SocketAddress clientAddress, ByteBuffer message) throws IOException; //clientAddress or clientChannel?
		
		void sendMessage(ByteBuffer message) throws IOException;

		ByteBuffer receiveMessage() throws IOException;

		//public void communicateWithClient() throws IOException;

		public Channel getChannel();
		public void configureServerSocket(Selector selector) throws IOException;
		// it shouldn't be object, but couldn't think of something better
		// message can be buffer or string
		
	//	public void initAndRegisterServer()
	}

	/**********************************************
	 * TCP Connection
	 **********************************************/
	private class TcpConnection implements Connection {
		private int port; 
		private ServerSocketChannel serverSocket;
		
		public TcpConnection(Selector selector, int port) throws IOException {
			serverSocket = ServerSocketChannel.open();
			this.port = port;
		}
		

		
		@Override
		public void configureServerSocket(Selector selector) throws IOException {
	    	serverSocket.bind(new InetSocketAddress(InetAddress.getLocalHost(), port));
	    	serverSocket.configureBlocking(false);
	    	serverSocket.register(selector, SelectionKey.OP_ACCEPT);
	    }

		@Override
		public Channel getChannel() {
			return serverSocket;
		}

		@Override
		public void sendMessage(ByteBuffer message) throws IOException {
			// TODO Auto-generated method stub
			
		}

		@Override
		public ByteBuffer receiveMessage() throws IOException {
			// TODO Auto-generated method stub
			return null;
		}


	}

	/**********************************************
	 * UDP Connection
	 **********************************************/
	private class UdpConnection implements Connection {
		private int port; 
		private DatagramChannel serverSocket;
		private SocketAddress clientAddress;
		
		
		public UdpConnection(Selector selector, int port) throws IOException {
			serverSocket = DatagramChannel.open();
			this.port = port;		
		}
		@Override
		public void sendMessage(ByteBuffer message) throws IOException {
			serverSocket.send(message, clientAddress);
		}
		
		@Override
		public ByteBuffer receiveMessage() throws IOException {
	        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
	    	while(null == this.clientAddress) {
	    		this.clientAddress = serverSocket.receive(buffer);
	    	}
	    	
			try {
				System.out.println("Server received from UDP client: " + ((Message<Integer, Message<String, Void>>)BytesUtil.toObject(buffer.array())).getData().getKey());
			} catch (Exception e) {
				System.err.println("Filed to print what server recieved");
			}
	    			
			return buffer;
		}
		
		@Override
		public void configureServerSocket(Selector selector) throws IOException {
	    	serverSocket.socket().bind(new InetSocketAddress(InetAddress.getLocalHost(),port));
	    	serverSocket.configureBlocking(false);
	    	serverSocket.register(selector, SelectionKey.OP_READ);
	    }
		

		@Override
		public Channel getChannel() {
			return serverSocket;
		}
	}

	/**********************************************
	 * Broadcast Connection
	 **********************************************/
	private class BroadcastConnection implements Connection {
		private int port; 
		private DatagramChannel serverSocket;
		
		public BroadcastConnection(Selector selector, int port) throws IOException {
			serverSocket = DatagramChannel.open();
			this.port = port;
		}
		
		
		@Override
		public void configureServerSocket(Selector selector) throws IOException {
	    	serverSocket.socket().bind(new InetSocketAddress(InetAddress.getByName("255.255.255.255"),port));
	    	serverSocket.setOption(StandardSocketOptions.SO_BROADCAST, true);
	    	serverSocket.configureBlocking(false);
	    	serverSocket.register(selector, SelectionKey.OP_READ);
	    }

		@Override
		public Channel getChannel() {
			return serverSocket;
		}

		@Override
		public void sendMessage(ByteBuffer message) throws IOException {
			// TODO Auto-generated method stub
			
		}

		@Override
		public ByteBuffer receiveMessage() throws IOException {
			// TODO Auto-generated method stub
			return null;
		}

	}

	/**********************************************
	 * Protocol
	 **********************************************/
	private interface Protocol<K, V> { 

		public void handleMessage(Connection serverSocket, Message<K, V> message) throws IOException;


	}

	/**********************************************
	 * Ping Pong Protocol
	 **********************************************/

	private class PingPongProtocol<K, V> implements Protocol<K, V> {
		
		@Override
		public void handleMessage(Connection serverSocket, Message<K, V> message) throws IOException {
			
			String clientMessage = (String) message.getKey();
			String serverResponse = getResponseAccordingToMessage(clientMessage);
			byte[] response = BytesUtil.toByteArray(serverResponse);  
		    serverSocket.sendMessage(ByteBuffer.wrap(response));
		}
		
		private String getResponseAccordingToMessage(String clientMessage) {
			String serverResponse;
		    if(clientMessage.equals("Pong1") | clientMessage.equals("Pong2")) {
		    	serverResponse = "Ping";
		    } else if (clientMessage.equals("Ping1") | clientMessage.equals("Ping2")) {
		    	serverResponse = "Pong";
		    } else {
		    	serverResponse = "Wrong";
		    }
		    
		    return serverResponse;
		}
	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {

		PingPongProtocol<String, Void> ppProtocol = new PingPongProtocol<String, Void>(); //dummy 
		private List<Protocol> protocolsList;
		
		private void handleMessage(ByteBuffer message, Connection currentConnection) throws ClassNotFoundException, IOException {

			Message<Integer, Message<String, Void>> clientMessage = (Message<Integer, Message<String, Void>>) BytesUtil.toObject(message.array());
			//ppProtocol = protocolsList.get(clientMessage.getKey()); - get the protocol user specified he works with
			ppProtocol.handleMessage(currentConnection, clientMessage.getData());
		}

		private void addProtocol(Protocol protocol) {
			protocolsList.add(protocol);
			protocolsList.add(new PingPongProtocol<String, Void>());

		}

		private void removeProtocol(Protocol protocol) {
			protocolsList.remove(protocol);
		}
	}
}