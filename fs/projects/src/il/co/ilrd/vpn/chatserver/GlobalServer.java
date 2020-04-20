package il.co.ilrd.vpn.chatserver;

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

public class GlobalServer {
    public static final int BUFFER_SIZE = 2048;

	private ConnectionHandler connectionHandler;
	private MessageHandler messageHandler;

	public GlobalServer() throws IOException {
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
		                	System.err.println("in acceptable");
		                	
		                	ServerSocketChannel tcpServer = (ServerSocketChannel) connectionsMap.get(currentChannel).getChannel();
		                	SocketChannel tcpClient = tcpServer.accept();
		                	
		                	tcpClient.configureBlocking(false);
		                	tcpClient.register(selector, SelectionKey.OP_READ);
		                	connectionsMap.put(tcpClient, connectionsMap.get(currentChannel));        	
		                }
		                
		                if(key.isValid() && key.isReadable()) {
		                	System.err.println("in readable");

		                	
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
		
		public String toString() {
			if(null != tcpPath) {
				return tcpPath.toString();
			}
			return udpPath.toString();
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
			serverSocket.bind(new InetSocketAddress(port));
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
		public void handleMessage(ClientInfo serverSocket, GlobalMessage<K, V> message) throws IOException;
	}

	/**********************************************
	 * Ping Pong Protocol
	 **********************************************/

	private class PingPongProtocol<K, V> implements Protocol<K, V> {
		@Override
		public void handleMessage(ClientInfo clientInfo, GlobalMessage<K, V> message) throws IOException {
			String clientMessage = (String) message.getKey();
			System.out.println("PingPongServer recieved: " + clientMessage + " ,from client: " + clientInfo.toString());
			String serverResponse = getResponseAccordingToMessage(clientMessage);
			byte[] response = BytesUtil.toByteArray(serverResponse);  
			clientInfo.connection.sendMessage(clientInfo, ByteBuffer.wrap(response));
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
	
	/**********************************************
	 * Chat Server Protocol
	 **********************************************/
	class ChatClientInfo {
		private String name;

		public ChatClientInfo(String name) {
			this.name = name;
		}
		
		public String getName() {
			return name;
		}
	}
	
	private interface ChatProtocolKeysHandler {
		public GlobalChatServerMessage apply(GlobalChatServerMessage clientMessage, ClientInfo clientInfo);
	}
	
	private class GlobalChatProtocol<K, V> implements Protocol<K, V> {			
		private Map<SocketChannel, ChatClientInfo> registeredClients = new HashMap<>();	
		private Map<GlobalChatProtocolKeys, ChatProtocolKeysHandler> chatMethodsMap = new HashMap<>();
		
		public GlobalChatProtocol() {
			chatMethodsMap.put(GlobalChatProtocolKeys.REGISTRATION_REQUEST, new RegistrationReqHandler());
			chatMethodsMap.put(GlobalChatProtocolKeys.REGISTRATION_ACK, new WrongKeyHandler());
			chatMethodsMap.put(GlobalChatProtocolKeys.REGISTRATION_REFUSE, new WrongKeyHandler());
			chatMethodsMap.put(GlobalChatProtocolKeys.MESSAGE, new ClientMessageHandler());
			chatMethodsMap.put(GlobalChatProtocolKeys.BROADCAST_MESSAGE, new WrongKeyHandler());
			chatMethodsMap.put(GlobalChatProtocolKeys.REMOVE_REQUEST, new RemoveReqHandler());
			chatMethodsMap.put(GlobalChatProtocolKeys.ERROR_MESSAGE, new WrongKeyHandler());
		}
		
		class RegistrationReqHandler implements ChatProtocolKeysHandler {
			@Override
			public GlobalChatServerMessage apply(GlobalChatServerMessage clientMessage, ClientInfo clientInfo) {
				if(true == checkIfNameIsTaken(clientMessage.getData())) {
					return new GlobalChatServerMessage(GlobalChatProtocolKeys.REGISTRATION_REFUSE, "Client with such name already exists");
				}
				
				if(true == checkIfSocketIsTaken(clientInfo.tcpPath)) {
					return new GlobalChatServerMessage(GlobalChatProtocolKeys.REGISTRATION_REFUSE, "Client with this socket already exists");
				}
				
				try {
					sendAllChatClients(new GlobalChatServerMessage(GlobalChatProtocolKeys.NEW_CLIENT_REGISTRATION, "New client registered: " + clientMessage.getData()));
				} catch (IOException e) {
					System.err.println("Couldn't send all clients the message");
				}

				registeredClients.put(clientInfo.tcpPath, new ChatClientInfo(clientMessage.getData()));
				
				return new GlobalChatServerMessage(GlobalChatProtocolKeys.REGISTRATION_ACK, clientMessage.getData() + " registered successfully");
			}
		}
		
		class WrongKeyHandler implements ChatProtocolKeysHandler {
			@Override
			public GlobalChatServerMessage apply(GlobalChatServerMessage clientMessage, ClientInfo clientInfo) {
				return new GlobalChatServerMessage(GlobalChatProtocolKeys.ERROR_MESSAGE, "Wrong key");
			}
		}
		
		class RemoveReqHandler implements ChatProtocolKeysHandler {
			@Override
			public GlobalChatServerMessage apply(GlobalChatServerMessage clientMessage, ClientInfo clientInfo) {			
				GlobalChatServerMessage errorToClient = checkIfNoSuchClient(clientInfo);
			
				if(null == errorToClient) {
					registeredClients.remove(clientInfo.tcpPath);
					try {
						sendAllChatClients(new GlobalChatServerMessage(GlobalChatProtocolKeys.MESSAGE, "Client left chat: " + clientMessage.getData()));
					} catch (IOException e) {
						System.err.println("Couldn't send all clients the message");
					}
				}
				
				return errorToClient;
			}
		}
		
		class ClientMessageHandler implements ChatProtocolKeysHandler {
			@Override
			public GlobalChatServerMessage apply(GlobalChatServerMessage clientMessage, ClientInfo clientInfo) {
				GlobalChatServerMessage errorToClient = checkIfNoSuchClient(clientInfo);

				if(null == errorToClient) {				
					try {
						sendAllChatClientsExcept(new GlobalChatServerMessage(GlobalChatProtocolKeys.BROADCAST_MESSAGE,registeredClients.get(clientInfo.tcpPath) + ": " + clientMessage.getData()), clientInfo.tcpPath);
					} catch (IOException e) {
						System.err.println("Couldn't send all clients the message");
					}
				}
				return errorToClient;
			}
		}
	
		@Override
		public void handleMessage(ClientInfo clientInfo, GlobalMessage<K, V>message) throws IOException {
			GlobalChatServerMessage chatMessage = (GlobalChatServerMessage)message;
			
			GlobalChatServerMessage replyToClient = chatMethodsMap.get(chatMessage.getKey()).apply(chatMessage, clientInfo);
			
			if(null != replyToClient) {
				GlobalServerMessage messageToSend = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, replyToClient);
				ByteBuffer buffer = ByteBuffer.wrap(BytesUtil.toByteArray(messageToSend));
				clientInfo.tcpPath.write(buffer);
			}
		}
	
		private void sendAllChatClients(GlobalChatServerMessage message) throws IOException {
			GlobalServerMessage messageToSend = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, message);
			ByteBuffer buffer = ByteBuffer.wrap(BytesUtil.toByteArray(messageToSend));
			for (SocketChannel clientChannel : registeredClients.keySet()) {
				clientChannel.write(buffer);
				buffer.clear();
			}
		}
		
		private void sendAllChatClientsExcept(GlobalChatServerMessage message, SocketChannel clientNotToSend) throws IOException {
			GlobalServerMessage messageToSend = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, message);
			ByteBuffer buffer = ByteBuffer.wrap(BytesUtil.toByteArray(messageToSend));
			for (SocketChannel otherClientChannel : registeredClients.keySet()) {
				if(!otherClientChannel.equals(clientNotToSend)) {					
					otherClientChannel.write(buffer);
					buffer.clear();
				}
			}
		}
		
		private boolean checkIfNameIsTaken(String name) {
			for (ChatClientInfo clientInfo : registeredClients.values()) {
				if(clientInfo.getName().equals(name)) {
					return true;
				}
			}
			return false;
		}
		
		private boolean checkIfSocketIsTaken(SocketChannel clientSocket) {
			if(null != registeredClients.get(clientSocket)) {
				return true;
			}
			return false;
		}
		
		private GlobalChatServerMessage checkIfNoSuchClient(ClientInfo clientInfo) {
			if(null == registeredClients.get(clientInfo.tcpPath)) {
				return new GlobalChatServerMessage(GlobalChatProtocolKeys.ERROR_MESSAGE, "You aren't registered");
			}
			return null;
		}
	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private Map<GlobalProtocolType, Protocol> protocolMap = new HashMap<>();
		
		public MessageHandler() {
			addProtocol(GlobalProtocolType.PINGPONG, new PingPongProtocol<String, Void>());
			addProtocol(GlobalProtocolType.CHAT_SERVER, new GlobalChatProtocol<>());
		}

		private void handleMessage(ByteBuffer message, ClientInfo currentConnection) throws ClassNotFoundException, IOException {
			GlobalServerMessage clientMessage = (GlobalServerMessage) BytesUtil.toObject(message.array());
			GlobalProtocolType index = clientMessage.getKey();
			protocolMap.get(index).handleMessage(currentConnection, clientMessage.getData());
		}

		private void addProtocol(GlobalProtocolType index, Protocol protocol) {
			protocolMap.put(index, protocol);
		}

		private void removeProtocol(Protocol protocol) {
			protocolMap.remove(protocol);
		}
	}
}