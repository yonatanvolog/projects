package il.co.ilrd.vpn.chatserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.Channel;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.ClosedSelectorException;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.ConcurrentModificationException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;



public class RunServerGal implements Runnable  {

	public static void main(String[] args) throws ClassNotFoundException, InterruptedException, IOException {
		RunServerGal server = new RunServerGal();
		server.addTcpConnection(GlobalProtocolPort.CHAT_PROTOCOL_PORT.getPort());
		System.out.println("before: new Thread(server).start();");		
		new Thread(server).start();	
		System.out.println("after: new Thread(server).start();");
		Thread.sleep(2000);
		//server.stopServer();
		//System.out.println("bye");\\
	}
	
	private static final String ALREADY_RUNNING = "server is already running, can't add new connections";
	private static final String INVALID_PORT = "this port number was already added: ";
	private ConnectionHandler connectionHandler = new ConnectionHandler();
	private MessageHandler messageHandler = new MessageHandler();
	private boolean isRunning = false;
	
	@Override
	public void run() {
		if(!isRunning) {
			isRunning = true;
			InputDetector inputDetector = new InputDetector();
			inputDetector.start();			
			try {		
				connectionHandler.startConnections();
			} catch (ClassNotFoundException | InterruptedException | IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void stopServer() throws IOException{		
		isRunning = false;
		connectionHandler.stopConnections();
	}

	public void addTcpConnection(int portNumber) {
		validateRequest(portNumber);		
		System.out.println("add TCP Connection");
		connectionHandler.addConnection(new TcpConnection(portNumber));		
	}

	public void addUdpConnection(int portNumber) {
		validateRequest(portNumber);		
		System.out.println("add UDP Connection");
		connectionHandler.addConnection(new UdpConnection(portNumber));			
	}

	public void addBroadcastConnection(int portNumber) {
		validateRequest(portNumber);		
		System.out.println("add Broadcast Connection");
		connectionHandler.addConnection(new BroadcastConnection(portNumber));	
	}
	
	private void validateRequest(int portNumber) {		
		if(isRunning) {
			throw new IllegalAccessError(ALREADY_RUNNING);						
		}		
		for(Connection connection : connectionHandler.connectionsList) {
			if(portNumber == connection.getPortNumber()) {
				throw new IllegalAccessError(INVALID_PORT + portNumber);
			}			
		}		
	}	
	
	/**********************************************
	 * Connection Handler
	 **********************************************/
	private class ConnectionHandler {
		private static final int BUFFER_SIZE = 2048;		
		private ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
		private List<Connection> connectionsList = new LinkedList<>();
		private Map<Channel, Connection> channelConnectionsMap = new HashMap<>();
		private Selector selector;

		private void startConnections() throws InterruptedException, ClassNotFoundException, IOException {			
			selector = Selector.open();
			initConnections();
			startSelectorWork(selector);
		}

		private void startSelectorWork(Selector selector) throws IOException, ClassNotFoundException {
			Channel currentChannel;
			SelectionKey key;
			
			while(isRunning) {			
				try {					
					if(0 == selector.select(10000)) {
						if(isRunning) {
							System.out.println("server is up and running");							
						}
					} else {
						Set<SelectionKey> selectedKeys = selector.selectedKeys();
						Iterator<SelectionKey> iter = selectedKeys.iterator();
						
						while(iter.hasNext()) {	
							key = iter.next();
							currentChannel = key.channel();
							
							if(key.isValid()) {
								if(key.isAcceptable()) {								
									registerTcpClient(selector, (ServerSocketChannel)currentChannel);
								}							
								if(key.isReadable()) {									
									channelConnectionsMap.get(currentChannel).handleChannel(currentChannel, buffer);							
								}							
								iter.remove();
							}
						}						
					}
				} catch (ClosedSelectorException | ConcurrentModificationException | ClosedChannelException e) {
					System.out.println("exiting");
					System.err.println("error: " + e);
				}
			}		
		}

		private void stopConnections() throws IOException {
			Iterator<SelectionKey> keys = selector.keys().iterator();
			
			while(keys.hasNext()) {
				Channel channelToClose = keys.next().channel();
				channelToClose.close();
			}			
			selector.close();			
		}

		private void addConnection(Connection connection) {
			connectionsList.add(connection);
		}

		private void initConnections() throws ClosedChannelException, IOException {
			for(Connection connection : connectionsList) {
				connection.registerChannel(selector);
				channelConnectionsMap.put(connection.getChannel(), connection);
			}			
		}

		private void registerTcpClient(Selector selector, ServerSocketChannel serverChannel) throws IOException {
			SocketChannel clientChannel = serverChannel.accept();
			channelConnectionsMap.put(clientChannel, channelConnectionsMap.get(serverChannel));
			clientChannel.configureBlocking(false);
			clientChannel.register(selector, SelectionKey.OP_READ);			
		}
	}

	/**********************************************
	 * Connection Interface
	 **********************************************/
	private interface Connection {
		public void registerChannel(Selector selector) throws ClosedChannelException, IOException;
		public void handleChannel(Channel currentChannel, ByteBuffer message) 
														throws IOException, ClassNotFoundException;	
		void sendMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException;
		public Channel getChannel();
		public int getPortNumber();
	}
	
	/**********************************************
	 * TCP Connection
	 **********************************************/
	private class TcpConnection implements Connection {
		private final int portNumber;
		private ServerSocketChannel tcpServerChannel;		
		
		public TcpConnection(int portNumber) {
			this.portNumber = portNumber;
		}

		@Override
		public void registerChannel(Selector selector) throws IOException {
			tcpServerChannel = ServerSocketChannel.open();
			tcpServerChannel.bind(new InetSocketAddress(portNumber));
			tcpServerChannel.configureBlocking(false);
			tcpServerChannel.register(selector, SelectionKey.OP_ACCEPT);		
		}

		@Override
		public void handleChannel(Channel tcpClientChannel, ByteBuffer buffers) 
				throws IOException, ClassNotFoundException {
			ByteBuffer buffer = ByteBuffer.allocate(1024);
			int bytes = ((SocketChannel)tcpClientChannel).read(buffer);
	    	if(-1 == bytes) {
	    		removeConnection(tcpClientChannel);	    		
	    	}	    	
	    	else {
	    		ClientInfo clientInfo = new ClientInfo(tcpClientChannel);
	    		messageHandler.handleMessage(buffer, clientInfo);
	    	}
		}

		@Override
		public void sendMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException {
			while(message.hasRemaining()) {
				((SocketChannel)clientInfo.getClientChannel()).write(message);
			}
			
			message.clear();
		}

		@Override
		public int getPortNumber() {
			return portNumber;
		}

		@Override
		public Channel getChannel() {
			return tcpServerChannel;
		}

		private void removeConnection(Channel tcpClientChannel) throws IOException {
			tcpClientChannel.close();	    		
    		connectionHandler.channelConnectionsMap.remove((SocketChannel)tcpClientChannel);			
		}
	}

	/**********************************************
	 * UDP Connection
	 **********************************************/
	private class UdpConnection implements Connection {
		private DatagramChannel UdpChannel;
		private final int portNumber;
				
		public UdpConnection(int portNumber) {
			this.portNumber = portNumber;			
		}

		@Override
		public void registerChannel(Selector selector) throws IOException {							
			UdpChannel = DatagramChannel.open();
			UdpChannel.socket().bind(new InetSocketAddress(portNumber));
			UdpChannel.configureBlocking(false);
			UdpChannel.register(selector, SelectionKey.OP_READ);						
		}

		@Override
		public void handleChannel(Channel currentChannel, ByteBuffer buffer) throws IOException, ClassNotFoundException {
			SocketAddress address = ((DatagramChannel)currentChannel).receive(buffer);
			if(null != address) {
				ClientInfo clientInfo = new ClientInfo(currentChannel, address);
				messageHandler.handleMessage(buffer, clientInfo);				
			}
		}

		@Override
		public void sendMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException {			
			UdpChannel.send(message, clientInfo.getUdpClientAddress());
			message.clear();
		}

		@Override
		public int getPortNumber() {
			return portNumber;
		}

		@Override
		public Channel getChannel() {
			return UdpChannel;
		}
	}

	/**********************************************
	 * Broadcast Connection
	 **********************************************/
	private class BroadcastConnection extends UdpConnection {
		public BroadcastConnection(int portNumber) {
			super(portNumber);
		}		
	}
	
	/***********************************************
	 * Client Info
	 **********************************************/
	private class ClientInfo {
		private Channel clientChannel;
		private SocketAddress udpClientAddress;
		
		public ClientInfo(Channel clientChannel) {
			this.clientChannel = clientChannel;
		}
		
		public ClientInfo(Channel clientChannel, SocketAddress udpClientAddress) {
			this.udpClientAddress = udpClientAddress;
			this.clientChannel = clientChannel;
		}

		public Channel getClientChannel() {
			return clientChannel;
		}

		public SocketAddress getUdpClientAddress() {
			return udpClientAddress;
		}	
	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private Map<Integer, Protocol> protocolMap = new HashMap<>();
		
		private MessageHandler() {
			addProtocol(new ChatProtocol());
		}		

		private void handleMessage(ByteBuffer messageBuffer, ClientInfo clientInfo) 
				throws IOException, ClassNotFoundException {			
			GlobalServerMessage message = (GlobalServerMessage) ObjectCovertUtil.toObject(messageBuffer.array());
			System.err.println("server recived : " + message);
			Protocol currentProtocol = protocolMap.get(message.getKey().ordinal());
			currentProtocol.handleMessage(clientInfo, message.getData());
		}

		private void addProtocol(Protocol protocol) {
			protocolMap.put(protocol.getKeyProtocol().ordinal(), protocol);			
		}
	}

	/**********************************************
	 * Protocol
	 **********************************************/
	private interface Protocol {
		public GlobalProtocolType getKeyProtocol();
		public void handleMessage(ClientInfo clientInfo, GlobalMessage<?, ?> message) 
				throws IOException, ClassNotFoundException;
	}

	/**********************************************
	 * chat Protocol
	 **********************************************/
	private class ChatProtocol implements Protocol {		
		private static final String ACCEPT_MESSAGE = "you just joined the chat";
		private static final String WELLCOME_MESSAGE = " just joined the chat";
		private static final String ALREADY_REGISTERED = "you already registered!";
		private static final String NOT_REGISTERED = "you are not registered!";
		private static final String INVALID_KEY_MESSAGE = "invalid key";
		private static final String INVALID_NAME = "try a different name";
		private static final String LEAVE_MESSAGE = " just left the chat";
		private static final int BUFFER_SIZE = 2048;
		
		private final GlobalProtocolType PROTOCOL_KEY = GlobalProtocolType.CHAT_SERVER;
		private LinkedList<ChatUserInfo> usersList = new LinkedList<>();
		private String senderName;
		private ChatUserInfo senderInfo;
		private ByteBuffer buffer;

		@Override
		public GlobalProtocolType getKeyProtocol() {
			return PROTOCOL_KEY;
		}

		@Override
		public void handleMessage(ClientInfo clientInfo, GlobalMessage<?, ?> message) 
				throws IOException, ClassNotFoundException {
			GlobalChatServerMessage innerMessage = (GlobalChatServerMessage) message;
			GlobalChatProtocolKeys actionRequested = innerMessage.getKey();
			System.err.println("\nchat server recived action Requested: " + actionRequested);
			
			if(actionRequested == GlobalChatProtocolKeys.REGISTRATION_REQUEST) {			
				handleRegistration(innerMessage, clientInfo);
								
			} else if(actionRequested == GlobalChatProtocolKeys.MESSAGE) {
				handleChatMessage(innerMessage, clientInfo);
				
			} else if(actionRequested == GlobalChatProtocolKeys.REMOVE_REQUEST) {
				handleRemoveRequest(clientInfo);
				
			} else {
				sendSingleUserResponseMessage(clientInfo, GlobalChatProtocolKeys.ERROR_MESSAGE, INVALID_KEY_MESSAGE);
			}			
		}		

		private void handleRegistration(GlobalChatServerMessage innerMessage, ClientInfo clientInfo) throws IOException {
			senderName = innerMessage.getData();
			
			if(isRegistered(clientInfo)) {
				sendSingleUserResponseMessage(clientInfo, GlobalChatProtocolKeys.ERROR_MESSAGE, ALREADY_REGISTERED);
			} 
			else if(!isNameValid(senderName)) {
				sendSingleUserResponseMessage(clientInfo, GlobalChatProtocolKeys.ERROR_MESSAGE, INVALID_NAME);
			} else {
				addChatUser(clientInfo, innerMessage);
				sendSingleUserResponseMessage(clientInfo, GlobalChatProtocolKeys.REGISTRATION_ACK, ACCEPT_MESSAGE);
				sendAllUsersExceptOneMessage(findChatClient(clientInfo), 
						GlobalChatProtocolKeys.NEW_CLIENT_REGISTRATION, 
											 senderName + WELLCOME_MESSAGE);
			}
		}

		private void handleChatMessage(GlobalChatServerMessage innerMessage, ClientInfo clientInfo) throws IOException {
			if(isRegistered(clientInfo)) {
				senderInfo = findChatClient(clientInfo);
				senderName = senderInfo.getName();
				String messageForUsers = senderName + ": " + innerMessage.getData();
				sendAllUsersExceptOneMessage(senderInfo, GlobalChatProtocolKeys.BROADCAST_MESSAGE, messageForUsers);
			} else {				
				sendSingleUserResponseMessage(clientInfo, GlobalChatProtocolKeys.ERROR_MESSAGE, NOT_REGISTERED);
			}							
		}

		private void handleRemoveRequest(ClientInfo clientInfo) throws IOException {
			if(isRegistered(clientInfo)) {
				senderInfo = findChatClient(clientInfo);
				senderName = senderInfo.getName();	
				usersList.remove(senderInfo);
				sendAllUsersExceptOneMessage(senderInfo, GlobalChatProtocolKeys.REMOVE_REQUEST, senderName + LEAVE_MESSAGE);
			}
		}
		
		private void sendSingleUserResponseMessage(ClientInfo clientInfo, GlobalChatProtocolKeys key, String dataToSend) throws IOException {
			GlobalServerMessage messageForUser = new GlobalServerMessage(PROTOCOL_KEY, new GlobalChatServerMessage(key, dataToSend));			
			System.out.println("server send to one user: " + messageForUser);
			buffer = setBuffer(messageForUser);			
			Connection connection = connectionHandler.channelConnectionsMap.get(clientInfo.getClientChannel());
			if(null != connection) {
				connection.sendMessage(clientInfo, buffer);				
			}
		}
		
		private void sendAllUsersExceptOneMessage(ChatUserInfo senderUserInfo, GlobalChatProtocolKeys key, String dataToSend) throws IOException {
			GlobalServerMessage messageToGroup = new GlobalServerMessage(PROTOCOL_KEY, new GlobalChatServerMessage(key, dataToSend));
			buffer = setBuffer(messageToGroup);
			senderName = senderUserInfo.getName();
			
			System.out.println("server send to group: " + messageToGroup);			
			for(ChatUserInfo chatUserInfo : usersList) {
				String userName = chatUserInfo.getName();		
				Connection connection = connectionHandler.channelConnectionsMap.get(chatUserInfo.getClientInfo().getClientChannel());
				Channel channel = chatUserInfo.getClientInfo().getClientChannel();				
				
				if(channel.isOpen() && !senderName.equals(userName)) {
					connection.sendMessage(chatUserInfo.getClientInfo(), buffer);
				} else if(null == connection) {
					handleRemoveRequest(chatUserInfo.getClientInfo());
				}
			}			
		}

		private ByteBuffer setBuffer(GlobalServerMessage messageToGroup) throws IOException {			
			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);			
			buffer.clear();
			buffer.put(ObjectCovertUtil.toByteArray(messageToGroup));
			buffer.flip();			
			return buffer;
		}
		
		private void addChatUser(ClientInfo clientInfo, GlobalChatServerMessage innerMessage) {			
			System.out.println("adding chat user");
			senderName = innerMessage.getData();
			usersList.add(new ChatUserInfo(senderName, clientInfo));				
		}
		
		private boolean isNameValid(String newName) {
			for(ChatUserInfo chatUserInfo : usersList) {
				if(chatUserInfo.getName().equals(newName)) {
					return false;
				}
			}		
			return true;			
		}
		
		private ChatUserInfo findChatClient(ClientInfo info) {
			for(ChatUserInfo chatUserInfo : usersList) {
				if(chatUserInfo.getClientInfo().getClientChannel().equals(info.getClientChannel())) {
					return chatUserInfo;
				}
			}
			return null;
		}
		
		private boolean isRegistered(ClientInfo clientInfo) {
			if(null == findChatClient(clientInfo)) {
				return false;				
			}			
			return true;
		}

		private class ChatUserInfo {
			private String name;
			private ClientInfo clientInfo;
			
			public ChatUserInfo(String name, ClientInfo clientInfo) {
				this.name = name;
				this.clientInfo = clientInfo;
			}

			public String getName() {
				return name;
			}

			public ClientInfo getClientInfo() {
				return clientInfo;
			}			
		}
	}	

	private class InputDetector extends Thread {
		private BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		
		@Override
		public void run() {			
			String input;			
			try {
				input = reader.readLine();
				while(!input.equals("exit")) {
					input = reader.readLine();
				}
				if(isRunning) {
					stopServer();					
				}
				
			} catch (IOException e) {
				e.printStackTrace();
			}
		}	
	}
}