package il.co.ilrd.pingpong.handlers;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
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

public class Server {
    public static final int BUFFER_SIZE = 1024;

	private ConnectionHandler connectionHandler;
	private MessageHandler messageHandler;

	public Server() throws IOException {
		connectionHandler = new ConnectionHandler();
		messageHandler = new MessageHandler();
	}
	
	public void startServer() throws IOException, ClassNotFoundException {
		new Thread(connectionHandler).start();
	}

	public void stopServer() throws IOException {
		connectionHandler.stopConnections();
		//connectionHandler.removeConnection()?
	}

	public void addTcpConnection(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.broadcastServersList)) {
			connectionHandler.addConnection
				(new TcpConnection(portNumber), connectionHandler.tcpServersList);
		}
	}
	
	public void addUdpConnection(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.broadcastServersList)) {
			connectionHandler.addConnection
				(new UdpConnection(portNumber), connectionHandler.udpServersList);
		}
	}
	
	public void addBroadcastConnection(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.broadcastServersList)) {
			connectionHandler.addConnection
				(new BroadcastConnection(portNumber), connectionHandler.broadcastServersList);
		}
	}
	
	private boolean connectionCanBeCreated(int portNumber, List<Connection> specificServerList) {
		boolean answer = true;
		for (Connection connection : specificServerList) {
			if(portNumber == connection.getPort()) {
				answer = false;
			}
		}
		return answer;
	}

	/**********************************************
	 * Connection Handler
	 **********************************************/
	private class ConnectionHandler implements Runnable{
    	private Selector selector;
		private boolean toContinue;
		private Map<Channel, Connection> connectionsMap = new HashMap<>();
		private List<Connection> tcpServersList;
		private List<Connection> udpServersList;
		private List<Connection> broadcastServersList;

		public ConnectionHandler() throws IOException {
			tcpServersList = new ArrayList<>();
			udpServersList = new ArrayList<>();
			broadcastServersList = new ArrayList<>();

			selector = Selector.open();
		}
		
		@Override
		public void run() {
			toContinue = true;
			try {
				configureExistingServerSockets();
		        System.out.println("Server started running");   
				while (toContinue) {     
					if(0 == selector.select(3000)) {
						System.out.println("Channels are not ready");
						continue;
					};	
					Set<SelectionKey> selectedKeys = selector.selectedKeys();
		            Iterator<SelectionKey> iter = selectedKeys.iterator();
		            
		            while (toContinue & iter.hasNext()) {		            	
		                SelectionKey key = iter.next();
		                Channel currentChannel = key.channel();
		 
		                if(key.isValid() && key.isAcceptable()) {
		                	ServerSocketChannel tcpServer = (ServerSocketChannel) connectionsMap.get(currentChannel).getChannel();
		                	SocketChannel tcpClient = tcpServer.accept();
		                	
		                	tcpClient.configureBlocking(false);
		                	tcpClient.register(selector, SelectionKey.OP_READ);
		                	connectionsMap.put(tcpClient, connectionsMap.get(currentChannel));        	
		                }
		                
		                if(key.isValid() && key.isReadable()) {	
	                		Connection currentConnection = connectionsMap.get(currentChannel);
	                		currentConnection.receiveMessage(currentChannel);
	                	}
	
		                iter.remove();
		            }
		        }
			} catch (Exception e) {
				if(toContinue) {
					throw new RuntimeException(e);
				}
			}
			System.out.println("Server terminated");
		}

		private void stopConnections() throws IOException {
			System.out.println("Closing server");
			toContinue = false;
			for(SelectionKey keyIter :selector.keys()) {
				keyIter.channel().close();
			}
			selector.close();
		}

		private void addConnection(Connection connection, List<Connection> specificServerList) {
			specificServerList.add(connection);
		}

		private void removeConnection(Connection connection , List<Connection> specificServerList) {
			specificServerList.remove(connection);
		}
		
		private void configureExistingServerSockets() throws IOException {
	        for (Connection serverConnection : tcpServersList) {
	        	serverConnection.configureServerSocket(selector);
	        	connectionsMap.put(serverConnection.getChannel(), serverConnection);
			}
	        
	        for (Connection serverConnection : udpServersList) {
	        	serverConnection.configureServerSocket(selector);
	        	connectionsMap.put(serverConnection.getChannel(), serverConnection);
			}
	        
	        for (Connection serverConnection : broadcastServersList) {
	        	serverConnection.configureServerSocket(selector);
	        	connectionsMap.put(serverConnection.getChannel(), serverConnection);
			}
		}
	}
	
	/**********************************************
	 * Client info
	 **********************************************/
	private class ClientInfo {
		private SocketChannel tcpPath;
		private SocketAddress udpPath;
		private Connection connection;
		
		public ClientInfo(SocketAddress udpPath, Connection connection) {
			this.udpPath = udpPath;
			this.connection = connection;
		}
		
		public ClientInfo(SocketChannel tcpPath, Connection connection) {
			this.tcpPath = tcpPath;
			this.connection = connection;
		}
	}
	
	/**********************************************
	 * Connection Interface
	 **********************************************/

	private interface Connection {
		public void sendMessage(ClientInfo client,ByteBuffer message) throws IOException;
		public void receiveMessage(Channel clientChannel) throws IOException, ClassNotFoundException;
		public Channel getChannel();
		public int getPort();
		public void configureServerSocket(Selector selector) throws IOException;
	}

	/**********************************************
	 * TCP Connection
	 **********************************************/
	private class TcpConnection implements Connection {
		private int port; 
		private ServerSocketChannel serverSocket;
		
		public TcpConnection(int port) throws IOException {
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
		public int getPort() {
			return port;
		}

		@Override
		public void sendMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException {			
			while(message.hasRemaining()) {
				clientInfo.tcpPath.write(message);
			}
		}

		@Override
		public void receiveMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
	        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
			
			if(-1 == ((SocketChannel)clientChannel).read(buffer)) {
	        	throw new IOException();
	        }
			
			try {
				System.out.println("Server received from TCP client: " + ((ServerMessage<ProtocolIndex, PingPongMessage>)BytesUtil.toObject(buffer.array())).getData().getKey());
			} catch (Exception e) {
				System.err.println("Failed to print what server recieved");
			}
			
			ClientInfo clientInfo = new ClientInfo((SocketChannel)clientChannel, this);
    		messageHandler.handleMessage(buffer, clientInfo);
		}
	}

	/**********************************************
	 * UDP Connection
	 **********************************************/
	private class UdpConnection implements Connection {
		private int port; 
		private DatagramChannel serverSocket;		
		
		public UdpConnection(int port) throws IOException {
			serverSocket = DatagramChannel.open();
			this.port = port;
		}

		@Override
		public void sendMessage(ClientInfo client, ByteBuffer message) throws IOException {
			serverSocket.send(message, client.udpPath);
		}
		
		@Override
		public void receiveMessage(Channel clientChannel) throws IOException, ClassNotFoundException {    
			SocketAddress clientAddress = null;
			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
	    	while(null == clientAddress) {
	    		clientAddress = serverSocket.receive(buffer);
	    	}

			try {
				System.out.println("Server received from UDP client: " + ((ServerMessage<ProtocolIndex, PingPongMessage>)BytesUtil.toObject(buffer.array())).getData().getKey());
			} catch (Exception e) {
				System.err.println("Failed to print what server recieved");
			}	
			
			ClientInfo clientInfo = new ClientInfo((SocketAddress)clientAddress, this);
    		messageHandler.handleMessage(buffer, clientInfo);
		}
		
		@Override
		public void configureServerSocket(Selector selector) throws IOException {
	    	serverSocket.socket().bind(new InetSocketAddress(port));
	    	serverSocket.configureBlocking(false);
	    	serverSocket.register(selector, SelectionKey.OP_READ);
	    }
		
		@Override
		public Channel getChannel() {
			return serverSocket;
		}

		@Override
		public int getPort() {
			return port;
		}
	}

	/**********************************************
	 * Broadcast Connection
	 **********************************************/
	private class BroadcastConnection extends UdpConnection {
		public BroadcastConnection(int port) throws IOException {
			super(port);
		}
	}

	/**********************************************
	 * Protocol
	 **********************************************/
	private interface Protocol<K, V> { 
		public void handleMessage(ClientInfo serverSocket, Message<K, V> message) throws IOException;
	}

	/**********************************************
	 * Ping Pong Protocol
	 **********************************************/

	private class PingPongProtocol<K, V> implements Protocol<K, V> {
		@Override
		public void handleMessage(ClientInfo clientInfo, Message<K, V> message) throws IOException {
			String clientMessage = (String) message.getKey();
			String serverResponse = getResponseAccordingToMessage(clientMessage);
			byte[] response = BytesUtil.toByteArray(serverResponse);  
			clientInfo.connection.sendMessage(clientInfo, ByteBuffer.wrap(response)); //?
		}
		
		private String getResponseAccordingToMessage(String clientMessage) {
			final String CLIENT_INPUT_VER1_1 = "Pong1";
			final String CLIENT_INPUT_VER1_2 = "Pong2";
			final String CLIENT_INPUT_VER2_1 = "Ping1";
			final String CLIENT_INPUT_VER2_2 = "Ping2";
			final String SERVER_OUTPUT_VER1 = "Ping";
			final String SERVER_OUTPUT_VER2 = "Pong";
			final String SERVER_OUTPUT_BAD = "Wrong";

			String serverResponse;
		    if(clientMessage.equals(CLIENT_INPUT_VER1_1) | clientMessage.equals(CLIENT_INPUT_VER1_2)) {
		    	serverResponse = SERVER_OUTPUT_VER1;
		    } else if (clientMessage.equals(CLIENT_INPUT_VER2_1) | clientMessage.equals(CLIENT_INPUT_VER2_2)) {
		    	serverResponse = SERVER_OUTPUT_VER2;
		    } else {
		    	serverResponse = SERVER_OUTPUT_BAD;
		    }
		    
		    return serverResponse;
		}
	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private Map<ProtocolIndex, Protocol> protocolMap = new HashMap<>();
		
		public MessageHandler() {
			addProtocol(ProtocolIndex.PingPong, new PingPongProtocol<String, Void>());
		}

		private void handleMessage(ByteBuffer message, ClientInfo currentConnection) throws ClassNotFoundException, IOException {
			ServerMessage<ProtocolIndex, PingPongMessage> clientMessage = (ServerMessage<ProtocolIndex, PingPongMessage>) BytesUtil.toObject(message.array());
			ProtocolIndex index = clientMessage.getKey();
			protocolMap.get(index).handleMessage(currentConnection, clientMessage.getData());
		}

		private void addProtocol(ProtocolIndex index, Protocol protocol) {
			protocolMap.put(index, protocol);
		}

		private void removeProtocol(Protocol protocol) {
			protocolMap.remove(protocol);
		}
	}
}