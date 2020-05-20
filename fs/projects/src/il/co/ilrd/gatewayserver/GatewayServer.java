package il.co.ilrd.gatewayserver;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
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
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.json.JSONObject;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import il.co.ilrd.DatabaseManagement.DatabaseManagement;
import il.co.ilrd.http_message.HttpBuilder;
import il.co.ilrd.http_message.HttpParser;
import il.co.ilrd.http_message.HttpStatusCode;
import il.co.ilrd.http_message.HttpVersion;
import il.co.ilrd.jarloader1.JarLoader;
import il.co.ilrd.jarloader1.SayHi;

public class GatewayServer {
	private static final int BUFFER_SIZE = 4096;
	private static final int DEFAULT_NUM_OF_THREADS = 1;
	private static final String COMMAND_KEY = "Commandkey";
	private static final String DATA = "Data";
	private static final int MAXIMUM_THREADS = 20;
	
	private ConnectionHandler connectionHandler;
	private MessageHandler messageHandler;	
	private ThreadPoolExecutor executor;
	private CMDFactory<FactoryCommand, String, Object> cmdFactory;
	private int numOfThreads;
	private HighLevelHttpServer highLevelHttpServer;
	private IotTaskCreator iotTaskCreator;

	public GatewayServer(int numOfThreads) throws ClassNotFoundException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException, IOException {
		this.numOfThreads = numOfThreads;
		initFactory();
		connectionHandler = new ConnectionHandler();
		messageHandler = new MessageHandler();
		iotTaskCreator = new IotTaskCreator();
	}
	
	public GatewayServer() throws IOException, ClassNotFoundException, NoSuchMethodException, SecurityException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException {
		this(DEFAULT_NUM_OF_THREADS);
	}
	
	public void setNumOfThreads(int numOfThreads) {
		executor.setCorePoolSize(numOfThreads);
	}
	
	public void start() throws IOException, ClassNotFoundException {
		initExecutor();
		new Thread(connectionHandler).start();
		if(null != highLevelHttpServer) {
			highLevelHttpServer.start();			
		}
	}
	
	private void initExecutor() {
		executor = new ThreadPoolExecutor(
					numOfThreads, 
					MAXIMUM_THREADS, 
					1, TimeUnit.SECONDS, 
					new LinkedBlockingQueue<Runnable>());
	}
	
	public void stop() throws IOException {
		connectionHandler.stopConnections();
		executor.shutdown();
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
	 * Factory
	 **********************************************/
	public interface FactoryCommand {
		public String run(Object data, DatabaseManagementInterface databaseManagement);
	}
	
	public interface FactoryCommandModifier {
		public void addToFactory();
	}
	
	public interface DatabaseManagementInterface {
		public void createTable(String sqlCommand) throws SQLException;
		public void createRow(String sqlCommand) throws SQLException;
		public void createIOTEvent(String rawData) throws SQLException;
	}
	
	private class FactoryCommandLoader {
		String interfaceName = "FactoryCommandModifier";
		String jarPath = "/home/student/Yonatan-Vologdin/fs/projects/bin/FactoryCommands.jar";

		public FactoryCommandLoader() {}
		
		public FactoryCommandLoader(String interfaceName, String jarPath) {
			this.interfaceName = interfaceName;
			this.jarPath = jarPath;
		}

		private void load() throws ClassNotFoundException, IOException, NoSuchMethodException, SecurityException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, InstantiationException {
			for(Class<?> classIter : JarLoader.load(interfaceName, jarPath)) {
				Method method = classIter.getMethod("addToFactory");
				method.invoke(classIter.getConstructor().newInstance());
			}
		}
	}
	
	Map<String, DatabaseManagement> dbManagementMap;
	FactoryCommandLoader commandLoader = new FactoryCommandLoader();
	
	private void initFactory() throws ClassNotFoundException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException, IOException {
		cmdFactory = CMDFactory.getInstance();
		dbManagementMap = new HashMap<>();
		commandLoader.load();
	}
	
	private static final String URL = "jdbc:mysql://localhost:3306/";
	private static final String USER_NAME = "root";
	private static final String PASSWORD = "";
	private static final String ERROR = "error";
	private final static String JSON_ERROR = "invalid json";

	private String createJsonResponse(String message1 ,String message2) {
		return "{" + "\"" + message1 + "\"" +":" + "\"" + message2 + "\"" + "}";
	}

	/**********************************************
	 * ServerConnection Interface
	 **********************************************/
	interface ServerConnection {
		public void initServerConnection(Selector selector) throws IOException;
		public void start();
		public void stop() throws IOException;
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException;
		public void sendResponseMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException;
		public Channel getChannel();
		public int getPort();
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
	 * Connection Handler
	 **********************************************/
	private class ConnectionHandler implements Runnable {
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
		                	System.err.println("is acceptable");
		                	ServerSocketChannel tcpServer = (ServerSocketChannel) connectionsMap.get(currentChannel).getChannel();
		                	SocketChannel tcpClient = tcpServer.accept();
		                	
		                	tcpClient.configureBlocking(false);
		                	tcpClient.register(selector, SelectionKey.OP_READ);
		                	connectionsMap.put(tcpClient, connectionsMap.get(currentChannel));        	
		                }
		                
		                if(key.isValid() && key.isReadable()) {
		                	System.err.println("is readable");
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
			
			if(null != highLevelHttpServer) {				
				highLevelHttpServer.stop();
			}
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
		public void initServerConnection(Selector selector) throws IOException {
	    	serverSocket.socket().bind(new InetSocketAddress(port));
	    	serverSocket.configureBlocking(false);
	    	serverSocket.register(selector, SelectionKey.OP_READ);
	    }
		
		@Override
		public void start() {
			// TODO Auto-generated method stub
		}

		@Override
		public void stop() throws IOException {
			serverSocket.close();
		}
		
		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException { 
			SocketAddress clientAddress = null;
			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
	    	while(null == clientAddress) {
	    		clientAddress = serverSocket.receive(buffer);
	    	}
			
			ClientInfo clientInfo = new ClientInfo((SocketAddress)clientAddress, this);
			messageHandler.handleMessage(buffer, clientInfo);
		}
		
		@Override
		public void sendResponseMessage(ClientInfo client, ByteBuffer message) throws IOException {
			serverSocket.send(message, client.udpPath);
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
		public void stop() throws IOException {
			serverSocket.close();
		}
		
		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
			
			if(-1 == ((SocketChannel)clientChannel).read(buffer)) {
				connectionHandler.closeAndRemoveClient(clientChannel);
	        	System.out.println("TCP client has closed");
	        } else {
	        	ClientInfo clientInfo = new ClientInfo((SocketChannel)clientChannel, this);
	    		messageHandler.handleMessage(buffer, clientInfo);
	        }
		}
		
		@Override
		public void sendResponseMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException {			
			sendTcpMessage(clientInfo, message);
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
		public void initServerConnection(Selector selector) throws IOException {
			initTcpServerConnection(selector, serverSocket, port);
		}
		
		@Override
		public void start() {
			// TODO Auto-generated method stub
		}

		@Override
		public void stop() throws IOException {
			serverSocket.close();
		}
		
		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
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

		@Override
		public void sendResponseMessage(ClientInfo clientInfo, ByteBuffer message) throws IOException {
			HttpVersion dummyVersion = HttpVersion.HTTP_1_0;
			HttpStatusCode dummyStatusCode = HttpStatusCode.OK;	//can be OK/ CREATED/ BAD REQUEST
			String dummyBody = bufferToString(message);			
			Map<String, String> dummyHeaders = createHeader(dummyBody.length()); //what headers do we return?
			String httpResponse = HttpBuilder.createHttpResponseMessage(dummyVersion, dummyStatusCode, dummyHeaders, dummyBody);
			
			sendTcpMessage(clientInfo, stringToBuffer(httpResponse));
		}
		
		@Override
		public Channel getChannel() {
			return serverSocket;
		}
		
		@Override
		public int getPort() {
			return port;
		}

		private Map<String, String> createHeader(int bodyLength) {
			Map<String, String> header = new HashMap<>();
			header.put("Connection", "close");
			header.put("Connection-Type", "application/json");
			header.put("Content-Length", String.valueOf(bodyLength));
			
			return header;
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
		public void initServerConnection(Selector selector) throws IOException {
			// TODO Auto-generated method stub
		}
		
		@Override
		public void start() {
			javaHttpServer.start();
		}

		@Override
		public void stop() {
			javaHttpServer.stop(0);
		}

		@Override
		public void handleRequestMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
			// TODO Auto-generated method stub
		}
		
		class RootHandler implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {				
				String body = getStringFromInputStream(exchange.getRequestBody());
	    		try {
					messageHandler.handleMessage(stringToBuffer(body), new ClientInfo(exchange, HighLevelHttpServer.this));
				} catch (ClassNotFoundException | IOException e) {
					throw new IOException(e);
				}
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
			return null;
		}

		@Override
		public int getPort() {
			return javaHttpServer.getAddress().getPort();
		}
	}
	
	/***********************************************
	 * Runnable Json
	 **********************************************/
	private class RunnableJsonRequest implements Runnable {
		String commandKey;
		JSONObject data;
		ClientInfo clientInfo;
		
		public RunnableJsonRequest(String commandKey, JSONObject data, ClientInfo clientInfo) {
			this.commandKey = commandKey;
			this.data = data;
			this.clientInfo = clientInfo;
		}
		
		@Override
		public void run() {	
			String dbName = data.getString("dbName");	
			try {
				addDbToMapIfNotFound(dbName);
				String response = cmdFactory.create(commandKey, data).run(data, dbManagementMap.get(dbName));
				System.out.println("The response would be" + response);
				clientInfo.connection.sendResponseMessage(clientInfo, stringToBuffer(response));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	private void addDbToMapIfNotFound(String databaseName) throws SQLException {
		if(null == dbManagementMap.get(databaseName)) {
			dbManagementMap.put(databaseName,
					  new DatabaseManagement(URL, USER_NAME, PASSWORD, databaseName));
		}
	}
	
	private class IotTaskCreator {
		public Runnable createIotTask(JSONObject json, ClientInfo clientInfo) throws IOException {
			String commandKeyAsString = null;
			JSONObject dataJson = null;
			try {
				commandKeyAsString = (String) json.get(COMMAND_KEY);
				dataJson = new JSONObject(json.get(DATA).toString());
			} catch (Exception e) {
				clientInfo.connection.sendResponseMessage(clientInfo, stringToBuffer(createJsonResponse(ERROR, JSON_ERROR)));
			}
			Runnable runnable = new RunnableJsonRequest(commandKeyAsString, dataJson, clientInfo);

			return runnable;	
		}
	}

	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {		
		private void handleMessage(ByteBuffer message, ClientInfo currentConnection) throws ClassNotFoundException, IOException {	
			JSONObject json = null;
			try {				
				json = new JSONObject(bufferToString(message));
			} catch (Exception e) {
				String response = createJsonResponse(ERROR, JSON_ERROR);
				currentConnection.connection.sendResponseMessage(currentConnection, stringToBuffer(response));
			}
			System.err.println("Received body is: \n" + json);
			Runnable runnableTask = iotTaskCreator.createIotTask(json, currentConnection);
			executor.submit(runnableTask); //do we need the returned Future?
		}
	}
	
	private static String bufferToString(ByteBuffer dataBuffer) {
		dataBuffer.clear();
		byte[] arr = new byte[dataBuffer.capacity()];
		dataBuffer.get(arr, 0, arr.length);
		dataBuffer.clear();
		
		return new String(arr, StandardCharsets.UTF_8);
	}
	
	private static ByteBuffer stringToBuffer(String message) {
		return ByteBuffer.wrap(message.getBytes(StandardCharsets.UTF_8));
	}
}