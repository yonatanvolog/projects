package il.co.ilrd.gatewayserver;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.Channel;
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

import org.json.JSONObject;

import il.co.ilrd.gatewayserver.CMDFactory.FactoryCommand;

//JSON:
//{
//	"Commandkey": keyValue,
//	"Data": data (the data can be another json)
//}

public class GatewayServer {
    public static final int BUFFER_SIZE = 4096;

	private ConnectionHandler connectionHandler;
	private MessageHandler messageHandler;

	
//	private ThreadPool threadPool;
	private CMDFactory cmdFactory = new CMDFactory<>();
	
	//private JsonToRunnableConvertor jsonToRunnableConvertor = new JsonToRunnableConverter();
	
	public GatewayServer() throws IOException {
		connectionHandler = new ConnectionHandler();
		messageHandler = new MessageHandler();
	}
	
	public void start() throws IOException, ClassNotFoundException {
		new Thread(connectionHandler).start();
//		if(null != sunHttpServer) {
//			sunHttpServer.start();			
//		}
	}

	public void stop() throws IOException {
		connectionHandler.stopConnections();
		//connectionHandler.removeConnection()?
	}

	public void addTcpServer(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.tcpServersList)) {
			connectionHandler.addConnection
				(new TcpServer(portNumber), connectionHandler.tcpServersList);
		}
	}
	
	public void addUdpServer(int portNumber) throws IOException {
//		if(connectionCanBeCreated(portNumber, connectionHandler.broadcastServersList)) {
//			connectionHandler.addConnection
//				(new UdpConnection(portNumber), connectionHandler.udpServersList);
//		}
	}
	
	private boolean connectionCanBeCreated(int portNumber, List<ServerConnection> specificServerList) {
		boolean answer = true;
		for (ServerConnection connection : specificServerList) {
			if(portNumber == connection.getPort()) {
				answer = false;
			}
		}
		return answer;
	}
	
	/**********************************************
	 * Sun HttpServer relation to server
	 **********************************************/
//	SunHttpServer sunHttpServer;
//		
//	public void addSunHttpConnection(int portNumber) throws IOException {
//		sunHttpServer = new SunHttpServer(portNumber);
//	}

	/**********************************************
	 * Connection Handler
	 **********************************************/
	private class ConnectionHandler implements Runnable{
    	private Selector selector;
		private boolean toContinue;
		private Map<Channel, ServerConnection> connectionsMap = new HashMap<>();
		private List<ServerConnection> tcpServersList;
		//private List<Connection> udpServersList;

		public ConnectionHandler() throws IOException {
			tcpServersList = new ArrayList<>();
			//udpServersList = new ArrayList<>();

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
		                	System.out.println("in isAcceptable");
		                	ServerSocketChannel tcpServer = (ServerSocketChannel) connectionsMap.get(currentChannel).getChannel();
		                	SocketChannel tcpClient = tcpServer.accept();
		                	
		                	tcpClient.configureBlocking(false);
		                	tcpClient.register(selector, SelectionKey.OP_READ);
		                	connectionsMap.put(tcpClient, connectionsMap.get(currentChannel));        	
		                }
		                
		                if(key.isValid() && key.isReadable()) {
		                	System.out.println("in isReadable");
		                	ServerConnection currentConnection = connectionsMap.get(currentChannel);
	                		currentConnection.handleRequestMessage(currentChannel);
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

		private void addConnection(ServerConnection connection, List<ServerConnection> specificServerList) {
			specificServerList.add(connection);
		}

		private void removeConnection(ServerConnection connection , List<ServerConnection> specificServerList) {
			specificServerList.remove(connection);
		}
		
		private void configureExistingServerSockets() throws IOException {
	        for (ServerConnection serverConnection : tcpServersList) {
	        	serverConnection.initServerConnection(selector);
	        	connectionsMap.put(serverConnection.getChannel(), serverConnection);
			}
	        
//	        for (ServerConnection serverConnection : udpServersList) {
//	        	serverConnection.initServerConnection(selector);
//	        	connectionsMap.put(serverConnection.getChannel(), serverConnection);
//			}
		}
	}
	
	/**********************************************
	 * Client info
	 **********************************************/
	private class ClientInfo {
		private SocketChannel tcpPath;
		private SocketAddress udpPath;
		private ServerConnection connection;
		
		public ClientInfo(SocketAddress udpPath, ServerConnection connection) {
			this.udpPath = udpPath;
			this.connection = connection;
		}
		
		public ClientInfo(SocketChannel tcpPath, ServerConnection connection) {
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
	 * ServerConnection Interface
	 **********************************************/
	interface ServerConnection {
		public void start();
		public void stop();
		public void initServerConnection(Selector selector) throws IOException;
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException;
		public void sendResponseMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException;
		
		public Channel getChannel();
		public int getPort();
	}
	/**********************************************
	 * TCP Connection
	 **********************************************/
	private class TcpServer implements ServerConnection {
		private int port; 
		private ServerSocketChannel serverSocket;
		
		public TcpServer(int port) throws IOException {
			serverSocket = ServerSocketChannel.open();
			this.port = port;
		}

		@Override
		public void initServerConnection(Selector selector) throws IOException {
			serverSocket.bind(new InetSocketAddress(port));
	    	serverSocket.configureBlocking(false);
	    	serverSocket.register(selector, SelectionKey.OP_ACCEPT);
	    }
		

		@Override
		public void start() {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void stop() {
			// TODO Auto-generated method stub
			
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
		public void sendResponseMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException {			
			while(message.hasRemaining()) {
				clientInfo.tcpPath.write(message);
			}
		}

		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
			System.out.println("in TcpServer handleRequestMessage");
	        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
			
			if(-1 == ((SocketChannel)clientChannel).read(buffer)) {
	        	throw new IOException();
	        }
			
			ClientInfo clientInfo = new ClientInfo((SocketChannel)clientChannel, this);
    		messageHandler.handleMessage(buffer, clientInfo);
		}

	}

	/***********************************************
	 * Runnable Json
	 **********************************************/
	private class RunnableJsonRequest implements Runnable {
		CommandKey commandKey;
		String data;
		ClientInfo clientInfo;
		
		public RunnableJsonRequest(CommandKey commandKey, String data, ClientInfo clientInfo) {
			this.commandKey = commandKey;
			this.data = data;
			this.clientInfo = clientInfo;
		}
		
		@Override
		public void run() {
			//go to cmd factory
			//create the right command with the key
			//execute created command with the data
			FactoryCommand command = (FactoryCommand) cmdFactory.exampleFactory.create(commandKey, data);
			command.run();
		}
	}
	
	private class JsonToRunnableConverter {
		public Runnable convertToRunnable(JSONObject json, ClientInfo clientInfo) {
			String commandKeyAsString = (String) json.get("Commandkey");
			CommandKey commandKey = findCommandKey(commandKeyAsString);
			String data = (String) json.get("Data");
			
			Runnable runnable = new RunnableJsonRequest(commandKey, data, clientInfo);
			
			return runnable;	
		}
	}

	private static JSONObject convertToJsonObject(String jsonAsString) {
		JSONObject jsonObject = null;
		try {
			jsonObject = new JSONObject(jsonAsString);				
		} catch (Exception e) {
			//parsing of json message might fail
			e.printStackTrace();
		}
		
		return jsonObject;
	}
	
	private static CommandKey findCommandKey(String commandKey) {
		for (CommandKey command : CommandKey.values()) {
		    if(command.toString().equals(commandKey)) {
		    	return command;
		    }
		}
		
		return null;
	}
	
	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		//private Map<ProtocolType, Protocol> protocolMap = new HashMap<>();
		
		public MessageHandler() {
			//addProtocol(ProtocolType.DATABASE_MANAGEMENT, new IOTProtocol<>());
		}

		private void handleMessage(ByteBuffer message, ClientInfo currentConnection) throws ClassNotFoundException, IOException {
			System.out.println("in MessageHandler handleMessage");
			printBuffer(message);
			
			JSONObject json = convertToJsonObject(getStringFromBuffer(message));
			new JsonToRunnableConverter().convertToRunnable(json, currentConnection).run();
		}
	}
	
	private static String getStringFromBuffer(ByteBuffer message) {
		try {
			return (String) BytesUtil.toObject(message.array());
		} catch (ClassNotFoundException | IOException e) {
			e.printStackTrace();
		}
		
		return null;
	}
	
	
	private static void printBuffer(ByteBuffer message) {
		String messageAsString = null;
		try {
			messageAsString = (String) BytesUtil.toObject(message.array());
		} catch (ClassNotFoundException | IOException e) {
			e.printStackTrace();
		}
		System.out.println("message recieved is:\n" + messageAsString);
	}
}























/**********************************************
 * UDP Connection
 **********************************************/
//private class UdpConnection implements Connection {
//	private int port; 
//	private DatagramChannel serverSocket;		
//	
//	public UdpConnection(int port) throws IOException {
//		serverSocket = DatagramChannel.open();
//		this.port = port;
//	}
//
//	@Override
//	public void sendMessage(ClientInfo client, ByteBuffer message) throws IOException {
//		serverSocket.send(message, client.udpPath);
//	}
//	
//	@Override
//	public void receiveMessage(Channel clientChannel) throws IOException, ClassNotFoundException {    
//		SocketAddress clientAddress = null;
//		ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
//    	while(null == clientAddress) {
//    		clientAddress = serverSocket.receive(buffer);
//    	}
//		
//		ClientInfo clientInfo = new ClientInfo((SocketAddress)clientAddress, this);
//		messageHandler.handleMessage(buffer, clientInfo);
//	}
//	
//	@Override
//	public void configureServerSocket(Selector selector) throws IOException {
//    	serverSocket.socket().bind(new InetSocketAddress(port));
//    	serverSocket.configureBlocking(false);
//    	serverSocket.register(selector, SelectionKey.OP_READ);
//    }
//	
//	@Override
//	public Channel getChannel() {
//		return serverSocket;
//	}
//
//	@Override
//	public int getPort() {
//		return port;
//	}
//}

