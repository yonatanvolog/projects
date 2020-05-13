package il.co.ilrd.gatewayserver;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.Channel;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.function.Function;

import org.json.JSONObject;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import il.co.ilrd.http_message.HttpBuilder;
import il.co.ilrd.http_message.HttpParser;
import il.co.ilrd.http_message.HttpStatusCode;
import il.co.ilrd.http_message.HttpVersion;
import il.co.ilrd.httpiotserver.BytesUtil;

//JSON:
//{
//	"Commandkey": keyValue,
//	"Data": data (the data can be another json)
//}

public class GatewayServer {
	private static final int BUFFER_SIZE = 4096;
	private static final int DEFAULT_NUM_OF_THREADS = 1;

	private ConnectionHandler connectionHandler;
	private MessageHandler messageHandler;	
	private ThreadPoolExecutor executor;
	private CMDFactory cmdFactory;
	private HighLevelHttpServer highLevelHttpServer;
	Integer numOfThreads;
//	private JsonToRunnableConvertor jsonToRunnableConvertor = new JsonToRunnableConverter();

	public GatewayServer() throws IOException {
		initExampleFactory();
		connectionHandler = new ConnectionHandler();
		messageHandler = new MessageHandler();
	}
	
	public void setNumOfThreads(int numOfThreads) {
		this.numOfThreads = numOfThreads;
	}
	
	public void start() throws IOException, ClassNotFoundException {
		if(null == numOfThreads) {
			executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(DEFAULT_NUM_OF_THREADS);
		} else {
			executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(numOfThreads);
		}
		
		new Thread(connectionHandler).start();
		if(null != highLevelHttpServer) {
			highLevelHttpServer.start();			
		}
	}

	public void stop() throws IOException {
		connectionHandler.stopConnections();
		if(null != highLevelHttpServer) {
			highLevelHttpServer.stop();			
		}
		//connectionHandler.removeConnection()?
	}

	public void addTcpServer(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.tcpServersList)) {
			connectionHandler.addConnection
				(new TcpServer(portNumber), connectionHandler.tcpServersList);
		}
	}
	
	public void addUdpServer(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.udpServersList)) {
			connectionHandler.addConnection
				(new UdpServer(portNumber), connectionHandler.udpServersList);
		}
	}
	
	public void addLowLevelHttpServer(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.tcpServersList)) {
			connectionHandler.addConnection
				(new LowLevelHttpServer(portNumber), connectionHandler.tcpServersList);
		}
	}
	
	public void addHighLevelHttpServer(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.tcpServersList)) {
			highLevelHttpServer = new HighLevelHttpServer(portNumber);
		}
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
	 * Example Factory
	 **********************************************/
	CMDFactory<FactoryCommand, CommandKey, Object> exampleFactory;

	private void initExampleFactory() {
		exampleFactory = new CMDFactory<>();

//		Function<Object, FactoryCommand> companyRegCtor = (CompanyRegistration) -> new CompanyRegistration();
//		Function<Object, FactoryCommand> productRegCtor = (ProductRegistration) -> new ProductRegistration();
//		Function<Object, FactoryCommand> iotUserRegCtor = (IotUserRegistration) -> new IotUserRegistration();
//		Function<Object, FactoryCommand> iotUpdateCtor = (IotUpdate) -> new IotUpdate();

		exampleFactory.add(CommandKey.COMPANY_REGISTRATION, (CompanyRegistration) -> new CompanyRegistration());
		exampleFactory.add(CommandKey.PRODUCT_REGISTRATION, (ProductRegistration) -> new ProductRegistration());
		exampleFactory.add(CommandKey.IOT_USER_REGISTRATION, (IotUserRegistration) -> new IotUserRegistration());
		exampleFactory.add(CommandKey.IOT_UPDATE, (IotUpdate) -> new IotUpdate());
	}
	
	private final String PREFIX = "method executed according to - ";
	private final String ACK = "response from server to - ";

	public interface FactoryCommand {
		public  void run(String data, ClientInfo clientInfo);
	}
	
	private class CompanyRegistration implements FactoryCommand {
		@Override
		public void run(String data, ClientInfo clientInfo) {
			//create proper json string
			System.out.println(PREFIX + "company registration, data is:" + data);
			String response = ACK + data;
			System.out.println("sending from server:\n" + response);
			try {
				clientInfo.connection.sendResponseMessage(clientInfo, stringToBuffer(response));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	private class ProductRegistration implements FactoryCommand {
		@Override
		public void run(String data, ClientInfo clientInfo) {
			System.out.println(PREFIX + "product registration, data is:" + data);
			String response = ACK + data;
			try {
				clientInfo.connection.sendResponseMessage(clientInfo, stringToBuffer(response));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	private class IotUserRegistration implements FactoryCommand {
		@Override
		public void run(String data, ClientInfo clientInfo) {
			System.out.println(PREFIX + "iot user registration, data is:" + data);
			String response = ACK + data;
			try {
				clientInfo.connection.sendResponseMessage(clientInfo, stringToBuffer(response));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	private class IotUpdate implements FactoryCommand {
		@Override
		public void run(String data, ClientInfo clientInfo) {
			System.out.println(PREFIX + "iot update, data is:" + data);
			String response = ACK + data;
			try {
				clientInfo.connection.sendResponseMessage(clientInfo, stringToBuffer(response));
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**********************************************
	 * Sun LowLevelHttpServer relation to server
	 **********************************************/
//	SunLowLevelHttpServer sunLowLevelHttpServer;
//		
//	public void addSunHttpConnection(int portNumber) throws IOException {
//		sunLowLevelHttpServer = new SunLowLevelHttpServer(portNumber);
//	}

	/**********************************************
	 * Connection Handler
	 **********************************************/
	private class ConnectionHandler implements Runnable{
    	private Selector selector;
		private boolean toContinue;
		private Map<Channel, ServerConnection> connectionsMap = new HashMap<>();
		private List<ServerConnection> tcpServersList;
		private List<ServerConnection> udpServersList;

		public ConnectionHandler() throws IOException {
			tcpServersList = new ArrayList<>();
			udpServersList = new ArrayList<>();

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
	        
	        for (ServerConnection serverConnection : udpServersList) {
	        	serverConnection.initServerConnection(selector);
	        	connectionsMap.put(serverConnection.getChannel(), serverConnection);
			}
		}
		
		public void closeAndRemoveClient(Channel clientChannel) throws IOException {
			clientChannel.close();
			ServerConnection clientConnection = connectionsMap.get(clientChannel);
			connectionsMap.remove(clientChannel);
			tcpServersList.remove(clientConnection);
		}
	}
	
	/**********************************************
	 * Client info
	 **********************************************/
	private class ClientInfo {
		private SocketChannel tcpPath;
		private SocketAddress udpPath;
		private ServerConnection connection;
		private HttpExchange exchange;
		
		public ClientInfo(SocketAddress udpPath, ServerConnection connection) {
			this.udpPath = udpPath;
			this.connection = connection;
		}
		
		public ClientInfo(SocketChannel tcpPath, ServerConnection connection) {
			this.tcpPath = tcpPath;
			this.connection = connection;
		}
		
		//might not need
		public ClientInfo(HttpExchange exchange) {
			this.exchange = exchange;
			//this.connection = connection;
		}
		
		public ClientInfo(HttpExchange exchange, ServerConnection connection) {
			this.exchange = exchange;
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
	 * TCP Server
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
			initTcpServerConnection(selector, serverSocket, port);
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
			sendTcpMessage(clientInfo, message);
		}

		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
//			System.out.println("in TcpServer handleRequestMessage");
//	        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
//			
//			if(-1 == ((SocketChannel)clientChannel).read(buffer)) {
//	        	throw new IOException();
//	        }
//			
//			ClientInfo clientInfo = new ClientInfo((SocketChannel)clientChannel, this);
//    		messageHandler.handleMessage(buffer, clientInfo);
			
			
			System.out.println("in TcpServer handleRequestMessage");

			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
			
			if(-1 == ((SocketChannel)clientChannel).read(buffer)) {
				connectionHandler.closeAndRemoveClient(clientChannel);
	        	System.out.println("TCP client has closed");
	        } else {
	        	ClientInfo clientInfo = new ClientInfo((SocketChannel)clientChannel, this);
	    		messageHandler.handleMessage(buffer, clientInfo);
	        }
		}
	}
	
	private void initTcpServerConnection(Selector selector, ServerSocketChannel serverSocket, int port) throws IOException{
		serverSocket.bind(new InetSocketAddress(port));
    	serverSocket.configureBlocking(false);
    	serverSocket.register(selector, SelectionKey.OP_ACCEPT);
	}
	
	private void sendTcpMessage(ClientInfo clientInfo,ByteBuffer message) throws IOException {
		while(message.hasRemaining()) {
			clientInfo.tcpPath.write(message);
		}
	}
	
	/**********************************************
	 * UDP Server
	 **********************************************/
	private class UdpServer implements ServerConnection {
		private int port; 
		private DatagramChannel serverSocket;		
		
		public UdpServer(int port) throws IOException {
			serverSocket = DatagramChannel.open();
			this.port = port;
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
		public void sendResponseMessage(ClientInfo client, ByteBuffer message) throws IOException {
			serverSocket.send(message, client.udpPath);
		}
		
		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException { 
			System.out.println("in UdpServer handleRequestMessage");

			SocketAddress clientAddress = null;
			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
	    	while(null == clientAddress) {
	    		clientAddress = serverSocket.receive(buffer);
	    	}
			
			ClientInfo clientInfo = new ClientInfo((SocketAddress)clientAddress, this);
			messageHandler.handleMessage(buffer, clientInfo);
		}
		
		@Override
		public void initServerConnection(Selector selector) throws IOException {
	    	serverSocket.socket().bind(new InetSocketAddress(port));
	    	serverSocket.configureBlocking(false);
	    	serverSocket.register(selector, SelectionKey.OP_READ);
	    }
	}
	
	/**********************************************
	 * Low Level HTTP Server
	 **********************************************/
	private class LowLevelHttpServer implements ServerConnection {
		private int port; 
		private ServerSocketChannel serverSocket;
		
		public LowLevelHttpServer(int port) throws IOException {
			serverSocket = ServerSocketChannel.open();
			this.port = port;
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
		public void initServerConnection(Selector selector) throws IOException {
			initTcpServerConnection(selector, serverSocket, port);
		}

		@Override
		public void sendResponseMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException {
			HttpVersion dummyVersion = HttpVersion.HTTP_1_0;
			HttpStatusCode dummyStatusCode = HttpStatusCode.OK;					//can be OK/ CREATED/ BAD REQUEST
			String dummyBody = bufferToString(message);			
			//String dummyBody = "{\"dummy1\": \"dummy2\"}";
			Map<String, String> dummyHeaders = createHeader(dummyBody.length()); //what headers do we return?
			
			String httpResponse = HttpBuilder.createHttpResponseMessage(dummyVersion, dummyStatusCode, dummyHeaders, dummyBody);
			System.out.println("response will be:\n" + httpResponse);
			System.out.println("end of response***********");

			sendTcpMessage(clientInfo, stringToBuffer(httpResponse));
		}

		private Map<String, String> createHeader(int bodyLength) {
			Map<String, String> header = new HashMap<>();
			header.put("Connection", "close");
			header.put("Connection-Type", "application/json");
			header.put("Content-Length", String.valueOf(bodyLength));
			
			return header;
		}
		
		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
			System.out.println("in LowLevelHttpServer handleRequestMessage");

			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
			
			if(-1 == ((SocketChannel)clientChannel).read(buffer)) {
				connectionHandler.closeAndRemoveClient(clientChannel);
	        	System.out.println("HTTP client has closed");
	        } else {
				ClientInfo clientInfo = new ClientInfo((SocketChannel)clientChannel, this);
				HttpParser parser = new HttpParser(bufferToString(buffer));				
	    		messageHandler.handleMessage(stringToBuffer(parser.getBody().getBody()), clientInfo);
	        }
	    }
	}
	
	/**********************************************
	 * High Level HTTP Server
	 **********************************************/	
	public class HighLevelHttpServer implements ServerConnection {
		private HttpServer javaHttpServer;		

		public HighLevelHttpServer(int portNumber) throws IOException {
			javaHttpServer = HttpServer.create(new InetSocketAddress(portNumber), 0);
			javaHttpServer.setExecutor(null);
			
			javaHttpServer.createContext("/", new RootHandler());
		}
		
		@Override
		public void start() {
			javaHttpServer.start();
		}

		class RootHandler implements HttpHandler {

			@Override
			public void handle(HttpExchange exchange) throws IOException {
				System.out.println("in HighLevelHttpServer handle");

				ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
				
				String body = getStringFromInputStream(exchange.getRequestBody());
				
	    		try {
					messageHandler.handleMessage(stringToBuffer(body), new ClientInfo(exchange, HighLevelHttpServer.this));
				} catch (ClassNotFoundException | IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				
//				if(-1 == ((SocketChannel)clientChannel).read(buffer)) {
//					connectionHandler.closeAndRemoveClient(clientChannel);
//		        	System.out.println("HTTP client has closed");
//		        } else {
//					ClientInfo clientInfo = new ClientInfo((SocketChannel)clientChannel, this);
//					HttpParser parser = new HttpParser(bufferToString(buffer));				
//		    		messageHandler.handleMessage(stringToBuffer(parser.getBody().getBody()), clientInfo);
//		        }
			}
		}
		
		private JSONObject getJsonBody(HttpExchange exchange) throws IOException {
			InputStream is = exchange.getRequestBody();
			String body = getStringFromInputStream(is);
					
			return new JSONObject(body);
		}
		
		private String getStringFromInputStream(InputStream inputStream) throws IOException {
			BufferedInputStream bis = new BufferedInputStream(inputStream);
			ByteArrayOutputStream buf = new ByteArrayOutputStream();
			int result = bis.read();
			while(result != -1) {
			    buf.write((byte) result);
			    result = bis.read();
			}
			return buf.toString("UTF-8");
		}

		@Override
		public void stop() {
			javaHttpServer.stop(0);
		}

		@Override
		public void initServerConnection(Selector selector) throws IOException {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void sendResponseMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException {
			HttpStatusCode dummyStatusCode = HttpStatusCode.OK;
			sendJsonMessage(clientInfo.exchange, dummyStatusCode, bufferToString(message));
		}
		
		private void sendJsonMessage(HttpExchange exchange, HttpStatusCode statusCode, String jsonMessage) throws IOException {
			int jsonLength = 0;
			if(null != jsonMessage) {
				jsonLength = jsonMessage.length();
			}
			exchange.sendResponseHeaders(statusCode.getCode(), jsonLength);
		    OutputStream os = exchange.getResponseBody();
		    if(null != jsonMessage) {
		    	os.write(jsonMessage.getBytes());
		    }
		    os.close();
		}

		@Override
		public Channel getChannel() {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public int getPort() {
			// TODO Auto-generated method stub
			return 0;
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
			
			//this should run in a thread from thread pool
			FactoryCommand command = (FactoryCommand) exampleFactory.create(commandKey, data);
			command.run(data, clientInfo);
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
			//printBuffer(message);
			
			JSONObject json = convertToJsonObject(bufferToString(message));
			
			//run via thread pool?
			Runnable runnableJsonRequest = new JsonToRunnableConverter().convertToRunnable(json, currentConnection);
			executor.submit(runnableJsonRequest); //do we need Future?
		}
	}
	
	private static String bufferToString(ByteBuffer dataBuffer) {
		dataBuffer.clear();
		byte[] arr = new byte[dataBuffer.capacity()];
		dataBuffer.get(arr, 0, arr.length);
		dataBuffer.clear();
		
		return new String(arr, StandardCharsets.UTF_8);
	}
	
	//old version, might not need anymore
//	private static String bufferToString(ByteBuffer dataBuffer) {
//		try {
//			return (String) BytesUtil.toObject(dataBuffer.array());
//		} catch (ClassNotFoundException | IOException e1) {
//			try {
//				dataBuffer.clear();
//				byte[] arr = new byte[dataBuffer.capacity()];
//				dataBuffer.get(arr, 0, arr.length);
//				dataBuffer.clear();
//				
//				return new String(arr, StandardCharsets.UTF_8);
//			} catch (Exception e2) {
//				e2.printStackTrace();
//			}
//		}
//		
//		return null;
//	}
	
	
	
//	private static String bufferToString2(ByteBuffer dataBuffer) {
//		dataBuffer.clear();
//		byte[] arr = new byte[dataBuffer.capacity()];
//		dataBuffer.get(arr, 0, arr.length);
//		dataBuffer.clear();
//		
//		return new String(arr, StandardCharsets.UTF_8);
//	}
	
	private static ByteBuffer stringToBuffer(String message) {
			//works with tcp/udp client
//		try {
//			return ByteBuffer.wrap(BytesUtil.toByteArray(message));
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
//		return null;
		
			//works with postman
		
		return ByteBuffer.wrap(message.getBytes(StandardCharsets.UTF_8));
		//return ByteBuffer.wrap(message.getBytes());
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