package il.co.ilrd.httpiotserver;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.Channel;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.json.JSONArray;
import org.json.JSONObject;

import il.co.ilrd.DatabaseManagement.DatabaseManagement;
import il.co.ilrd.http_message.HttpBuilder;
import il.co.ilrd.http_message.HttpMethod;
import il.co.ilrd.http_message.HttpParser;
import il.co.ilrd.http_message.HttpStatusCode;
import il.co.ilrd.http_message.HttpVersion;

public class HttpServer {
    public static final int BUFFER_SIZE = 4096;

	private ConnectionHandler connectionHandler;
	private MessageHandler messageHandler;

	public HttpServer() throws IOException {
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
	
	public void addHttpConnection(int portNumber) throws IOException {
		if(connectionCanBeCreated(portNumber, connectionHandler.broadcastServersList)) {
			connectionHandler.addConnection
				(new HttpConnection(portNumber), connectionHandler.tcpServersList);
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

		public void closeAndRemoveClient(Channel clientChannel) throws IOException {
			clientChannel.close();
			Connection clientConnection = connectionsMap.get(clientChannel);
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
	 * HTTP Connection
	 **********************************************/
	private class HttpConnection extends TcpConnection {
		
		public HttpConnection(int port) throws IOException {
			super(port);
		}
		
		@Override
		public void receiveMessage(Channel clientChannel) throws IOException, ClassNotFoundException {
			ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
			
			if(-1 == ((SocketChannel)clientChannel).read(buffer)) {
				connectionHandler.closeAndRemoveClient(clientChannel);
	        	System.out.println("HTTP client has closed");
	        } else {
				buffer = wrapInServerMessage(buffer);
				ClientInfo clientInfo = new ClientInfo((SocketChannel)clientChannel, this);
	    		messageHandler.handleMessage(buffer, clientInfo);
	        }
	    }
		
		private ByteBuffer wrapInServerMessage(ByteBuffer buffer) throws IOException, ClassNotFoundException {
			String rawHttpMessage = getMessageFromBuffer(buffer);
			System.out.println("HttpConnection recieved: \n" + rawHttpMessage);
			HTTPMessage httpMessage = new HTTPMessage(null, rawHttpMessage);
			ServerMessage serverMessage = new ServerMessage(ProtocolType.DB_HTTP, httpMessage);

			return ByteBuffer.wrap(BytesUtil.toByteArray(serverMessage));
		}
		
		private String getMessageFromBuffer(ByteBuffer dataBuffer) {
			dataBuffer.clear();
			byte[] arr = new byte[dataBuffer.capacity()];
			dataBuffer.get(arr, 0, arr.length);
			dataBuffer.clear();
			
			return new String(arr, StandardCharsets.UTF_8);
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
		public void handleMessage(ClientInfo serverSocket, Message<K, V> message) throws IOException;
	}
	
	/**********************************************
	 * Ping Pong Protocol
	 **********************************************/

	private class PingPongProtocol<K, V> implements Protocol<K, V> {
		@Override
		public void handleMessage(ClientInfo clientInfo, Message<K, V> message) throws IOException {
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
	 * HTTP Protocol
	 **********************************************/
	private interface HttpProtocolKeysHandler {
		public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList);
	}
	
	private interface HTTPMethodHandler {
		public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException;
		public String chooseAction(ClientInfo clientInfo) throws SQLException;
	}
	
	private class HttpProtocol<K, V> implements Protocol<K, V> {
		private static final String URL = "jdbc:mysql://localhost:3306/";
		private static final String USER_NAME = "root";
		private static final String PASSWORD = "";
		final String INVALID_HTTP_METHOD = "Invalid http method";
		final String MISSING_PARAMS = "Missing parameters";
		final String NOT_IMPLEMENTED = "{\"errorMessage\":{\"not implemented\"}";

		private Map<DatabaseKeys, HttpProtocolKeysHandler> iotMethodsMap = new HashMap<>();
		private Map<String, DatabaseManagement> companyDatabases = new HashMap<>();
		private Map<String, DatabaseKeys> databaseKeysMap = new HashMap<>();
		private Map<HttpMethod, HTTPMethodHandler> httpMethodsHandlersMap = new HashMap<>();
		private Map<DatabaseKeys, JsonResponses> responsesMap = new HashMap<>();
		private MessageParser parser;
		
		public HttpProtocol() {			
			initIotMethodsMap();
			initHttpMethodsHandlersMap();
			initDatabaseKeysMap();
			initResponsesMap();
		}
		
		public class CreateCompanyDatabaseHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				return createAckMessage(databaseName, "database created");
			}
		}
		
		public class CreateTableHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				try {
					companyDatabases.get(databaseName).createTable(paramList.get(0).toString());
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, "table created");
			}
		}
		
		public class DeleteTableHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				try {
					companyDatabases.get(databaseName).deleteTable(paramList.get(0).toString());
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, "table deleted");
			}
		}
		
		public class CreateIotEventHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				try {
					companyDatabases.get(databaseName).createIOTEvent(paramList.get(0).toString());
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, "IOT event created");
			}
		}
		
		public class CreateRowHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				try {
					companyDatabases.get(databaseName).createRow(paramList.get(0).toString());
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, "row created");
			}
		}
		
		public class ReadRowHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				List<Object> returnList;
				try {
					String tableName = (String)paramList.get(0);
					String primaryKeyColumnName = (String)paramList.get(1);
					Object primaryKeyValue = paramList.get(2);
					returnList = companyDatabases.get(databaseName).readRow(tableName, primaryKeyColumnName, primaryKeyValue);
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, returnList);
			}
		}
		
		public class ReadFieldByNameHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				Object returnValue;
				try {
					String tableName = (String)paramList.get(0);
					String primaryKeyColumnName = (String)paramList.get(1);
					Object primaryKey = paramList.get(2);
					String columnName = (String)paramList.get(3);		
					returnValue = companyDatabases.get(databaseName).readField(tableName, primaryKeyColumnName, primaryKey, columnName);
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, returnValue);
			}
		}
		
		public class ReadFieldByIndexHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				Object returnValue;
				try {
					String tableName = (String)paramList.get(0);
					String primaryKeyColumnName = (String)paramList.get(1);
					Object primaryKey = paramList.get(2);
					int columnIndex = Integer.parseInt((String) paramList.get(3));
					returnValue = companyDatabases.get(databaseName).readField(tableName, primaryKeyColumnName, primaryKey, columnIndex);
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, returnValue);
			}
		}
		
		public class UpdateFieldByNameHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				try {
					String tableName = (String)paramList.get(0);
					String primaryKeyColumnName = (String)paramList.get(1);
					Object primaryKey = paramList.get(2);
					String columnName = (String)paramList.get(3);
					Object newValue = paramList.get(4);
					companyDatabases.get(databaseName).updateField(tableName, primaryKeyColumnName, primaryKey, columnName, newValue);
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, "field updated");
			}
		}
		
		public class UpdateFieldByIndexHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				try {
					String tableName = (String)paramList.get(0);
					String primaryKeyColumnName = (String)paramList.get(1);
					Object primaryKey = paramList.get(2);
					int columnIndex = Integer.parseInt((String) paramList.get(3));
					Object newValue = paramList.get(4);
					companyDatabases.get(databaseName).updateField(tableName, primaryKeyColumnName, primaryKey, columnIndex, newValue);
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, "field updated");
			}
		}
		
		public class DeleteRowHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				try {
					String tableName = (String)paramList.get(0);
					String primaryKeyColumnName = (String)paramList.get(1);
					Object primaryKey = paramList.get(2);
					companyDatabases.get(databaseName).deleteRow(tableName, primaryKeyColumnName, primaryKey);
				} catch (Exception e) {
					return createErrorMessage(databaseName, e);
				}
				
				return createAckMessage(databaseName, "row deleted");
			}
		}
		
		public class InvalidKeyHandler implements HttpProtocolKeysHandler {
			@Override
			public DatabaseManagementMessage apply(ClientInfo clientInfo, String databaseName ,List<Object> paramList) {
				return createErrorMessage(databaseName, "invalid key");
			}
		}

		@Override
		public void handleMessage(ClientInfo clientInfo, Message<K, V> message) throws IOException {
			HTTPMessage httpMessage = (HTTPMessage)message;
			try {
				openMessage(httpMessage, clientInfo);
			} catch (Exception e) {
				String json = createJsonString(DatabaseKeys.ERROR_MESSAGE, "openMessage" + e.toString());
				sendMessage(HttpStatusCode.BAD_REQUEST, json, clientInfo);
			}
		}
		
		private void openMessage(HTTPMessage httpMessage, ClientInfo clientInfo) throws SQLException {
			if (MessageParser.isOptions(httpMessage.getData())) {
				httpMethodsHandlersMap.get(HttpMethod.OPTIONS).handleMessage(clientInfo, null);
			} else {
				parser = new MessageParser(httpMessage.getData());
				createDatabaseIfNotInMap(parser.get(UrlKeys.DATABASE_NAME.asString()));
				httpMethodsHandlersMap.get(parser.getHttpMethod()).handleMessage(clientInfo, httpMessage);			
			}
		}

		private void createDatabaseIfNotInMap(String databaseName) throws SQLException {
			if(null == 	companyDatabases.get(databaseName)) {
				DatabaseManagement dbManager = new DatabaseManagement(URL, USER_NAME, PASSWORD, databaseName);
				companyDatabases.put(databaseName, dbManager);
			}
		}
		
		private DatabaseManagementMessage createErrorMessage(String databaseName, Object exception) {
			List<Object> errorDescription = new ArrayList<>();
			errorDescription.add(exception);
			
			return new DatabaseManagementMessage(new ActionTypeKey(databaseName, DatabaseKeys.ERROR_MESSAGE), errorDescription);
		}
	
		private DatabaseManagementMessage createAckMessage(String databaseName, Object value) {
			List<Object> returnList = new ArrayList<>();
			returnList.add(value);
			
			return new DatabaseManagementMessage(new ActionTypeKey(databaseName, DatabaseKeys.ACK_MESSAGE), returnList);
		}
		
		private DatabaseManagementMessage createAckMessage(String databaseName, List<Object> returnList) {
			return new DatabaseManagementMessage(new ActionTypeKey(databaseName, DatabaseKeys.ACK_MESSAGE), returnList);
		}
		
		
		private Map<String, String> createHeader(int bodyLength) {
			Map<String, String> header = new HashMap<>();
			header.put("Connection", "close");
			header.put("Connection-Type", "application/json");
			header.put("Content-Length", String.valueOf(bodyLength));
			
			return header;
		}
		
		private Map<String, String> createOptionsHeader(int bodyLength) {
			Map<String, String> header = new HashMap<>();
			header.put("Connection", "close");
			header.put("Connection-Type", "application/json");
			header.put("Allow", "[GET, POST, PUT, DELETE]");
			
			return header;
		}

		private void initIotMethodsMap() {
			iotMethodsMap.put(DatabaseKeys.CREATE_COMPANY_DATABASE, new CreateCompanyDatabaseHandler());
			iotMethodsMap.put(DatabaseKeys.CREATE_TABLE, new CreateTableHandler());
			iotMethodsMap.put(DatabaseKeys.DELETE_TABLE, new DeleteTableHandler());
			iotMethodsMap.put(DatabaseKeys.CREATE_IOT_EVENT, new CreateIotEventHandler());
			iotMethodsMap.put(DatabaseKeys.CREATE_ROW, new CreateRowHandler());
			iotMethodsMap.put(DatabaseKeys.READ_ROW, new ReadRowHandler());
			iotMethodsMap.put(DatabaseKeys.READ_FIELD_BY_NAME, new ReadFieldByNameHandler());
			iotMethodsMap.put(DatabaseKeys.READ_FIELD_BY_INDEX, new ReadFieldByIndexHandler());
			iotMethodsMap.put(DatabaseKeys.UPDATE_FIELD_BY_NAME, new UpdateFieldByNameHandler());
			iotMethodsMap.put(DatabaseKeys.UPDATE_FIELD_BY_INDEX, new UpdateFieldByIndexHandler());
			iotMethodsMap.put(DatabaseKeys.DELETE_ROW, new DeleteRowHandler());
			iotMethodsMap.put(DatabaseKeys.ERROR_MESSAGE, new InvalidKeyHandler());
			iotMethodsMap.put(DatabaseKeys.ACK_MESSAGE, new InvalidKeyHandler());
		}
		
		private void initHttpMethodsHandlersMap() {
			httpMethodsHandlersMap.put(HttpMethod.GET, new HttpGetMethodHandler());
			httpMethodsHandlersMap.put(HttpMethod.POST, new HttpPostMethodHandler());
			httpMethodsHandlersMap.put(HttpMethod.PUT, new HttpPutMethodHandler());
			httpMethodsHandlersMap.put(HttpMethod.DELETE, new HttpDeleteMethodHandler());
			httpMethodsHandlersMap.put(HttpMethod.OPTIONS, new HttpOptionsMethodHandler());
			httpMethodsHandlersMap.put(HttpMethod.TRACE, new HttpTraceMethodHandler());
			httpMethodsHandlersMap.put(HttpMethod.PATCH, new HttpPatchMethodHandler());
			httpMethodsHandlersMap.put(HttpMethod.HEAD, new HttpHeadMethodHandler());
		}
		
		private void initDatabaseKeysMap() {
			for (DatabaseKeys key : DatabaseKeys.values()) { 
				databaseKeysMap.put(key.toString(), key);
	        }
		}
		
		private void initResponsesMap() {
			for (DatabaseKeys key : DatabaseKeys.values()) {
				for (JsonResponses response : JsonResponses.values()) { 
					if(key.toString().equals(response.toString())) {
						responsesMap.put(key, response);
					}
		        }
	        }
		}
		
		/**********************************************
		 * Http Get Method Handler Class
		 **********************************************/
		private class HttpGetMethodHandler implements HTTPMethodHandler {
			@Override
			public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException {
				String json = chooseAction(clientInfo);
				try {
					processResponseMsg(json, HttpStatusCode.OK, clientInfo);
				} catch (IOException e) {
					throw new SQLException(e);
				}
			}

			@Override
			public String chooseAction(ClientInfo clientInfo) throws SQLException {
				if(null == parser.getDatabaseMethod()) {
					return createJsonString(DatabaseKeys.ERROR_MESSAGE, INVALID_HTTP_METHOD);
				} else {
					switch(parser.getDatabaseMethod()) {
						case READ_ROW:
							return readRow(clientInfo, parser.getDatabaseMethod());
						case READ_FIELD_BY_NAME:
							return readFieldByName(clientInfo, parser.getDatabaseMethod());
						case READ_FIELD_BY_INDEX:
							return readFieldByIndex(clientInfo, parser.getDatabaseMethod());
						default:
							return createJsonString(DatabaseKeys.ERROR_MESSAGE, INVALID_HTTP_METHOD);
					}
				}
			}
			private String readRow(ClientInfo clientInfo, DatabaseKeys iotMethod) throws SQLException {
				List<Object> paramList = new ArrayList<>();
				paramList.add(0, parser.get(UrlKeys.TABLENAME.asString()));
    			paramList.add(1, parser.get(JsonRequestKeys.PK_NAME.asString()));
    			paramList.add(2, parser.get(JsonRequestKeys.PK_VALUE.asString()));
    			
    			return getResponseJsonString(clientInfo, iotMethod, paramList);
			}	
			
			private String readFieldByName(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
				paramList.add(0, parser.get(UrlKeys.TABLENAME.asString()));
    			paramList.add(1, parser.get(JsonRequestKeys.PK_NAME.asString()));
    			paramList.add(2, parser.get(JsonRequestKeys.PK_VALUE.asString()));
    			paramList.add(3, parser.get(JsonRequestKeys.COLUMN_NAME.asString()));
    			
    			return getResponseJsonString(clientInfo, iotMethod, paramList);
			}
			
			private String readFieldByIndex(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
				paramList.add(0, parser.get(UrlKeys.TABLENAME.asString()));
    			paramList.add(1, parser.get(JsonRequestKeys.PK_NAME.asString()));
    			paramList.add(2, parser.get(JsonRequestKeys.PK_VALUE.asString()));
    			paramList.add(3, parser.get(JsonRequestKeys.COLUMN_INDEX.asString()));

    			return getResponseJsonString(clientInfo, iotMethod, paramList);
			}
		}
		
		/**********************************************
		 * Http Post Method Handler Class
		 **********************************************/
		private class HttpPostMethodHandler implements HTTPMethodHandler {
			@Override
			public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException {
				String json = chooseAction(clientInfo);
				try {
					processResponseMsg(json, HttpStatusCode.CREATED, clientInfo);
				} catch (IOException e) {
					throw new SQLException(e);
				}
			}

			@Override
			public String chooseAction(ClientInfo clientInfo) throws SQLException {
				if(null == parser.getDatabaseMethod()) {
					return createJsonString(DatabaseKeys.ERROR_MESSAGE, INVALID_HTTP_METHOD);
				} else {
					switch(parser.getDatabaseMethod()) {
						case CREATE_TABLE:
							return createTable(clientInfo, parser.getDatabaseMethod());
						case CREATE_ROW:
							return createRow(clientInfo, parser.getDatabaseMethod());
						case CREATE_IOT_EVENT:
							return createIotEvent(clientInfo, parser.getDatabaseMethod());
						default:
							return createJsonString(DatabaseKeys.ERROR_MESSAGE, INVALID_HTTP_METHOD);
					}
				}
			}
			
			private String createTable(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
    			paramList.add(0, parser.get(JsonRequestKeys.SQL_COMMAND.asString()));

    			return getResponseJsonString(clientInfo, iotMethod, paramList);
			}

			private String createRow(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
    			paramList.add(0, parser.get(JsonRequestKeys.SQL_COMMAND.asString()));

    			return getResponseJsonString(clientInfo, iotMethod, paramList);

			}

			private String createIotEvent(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
    			paramList.add(0, parser.get(JsonRequestKeys.RAW_DATA.asString()));

    			return getResponseJsonString(clientInfo, iotMethod, paramList);
			}
		}
		
		/**********************************************
		 * Http Put Method Handler Class
		 **********************************************/
		private class HttpPutMethodHandler implements HTTPMethodHandler {
			@Override
			public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException {
				String json = chooseAction(clientInfo);
				try {
					processResponseMsg(json, HttpStatusCode.OK, clientInfo);
				} catch (IOException e) {
					throw new SQLException(e);
				}
			}

			@Override
			public String chooseAction(ClientInfo clientInfo) throws SQLException {
				if(null == parser.getDatabaseMethod()) {
					return createJsonString(DatabaseKeys.ERROR_MESSAGE, INVALID_HTTP_METHOD);
				} else {
					switch(parser.getDatabaseMethod()) {
						case UPDATE_FIELD_BY_NAME:
							return updateFieldByName(clientInfo, parser.getDatabaseMethod());
						case UPDATE_FIELD_BY_INDEX:
							return updateFieldByIndex(clientInfo, parser.getDatabaseMethod());
						default:
							return createJsonString(DatabaseKeys.ERROR_MESSAGE, INVALID_HTTP_METHOD);
					}
				}
			}
			
			private String updateFieldByName(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
				paramList.add(0, parser.get(UrlKeys.TABLENAME.asString()));
    			paramList.add(1, parser.get(JsonRequestKeys.PRIMARY_KEY_NAME.asString()));
    			paramList.add(2, parser.get(JsonRequestKeys.PRIMARY_KEY_VALUE.asString()));
    			paramList.add(3, parser.get(JsonRequestKeys.COLUMN_NAME.asString()));
    			paramList.add(4, parser.get(JsonRequestKeys.NEW_VALUE.asString()));

    			return getResponseJsonString(clientInfo, iotMethod, paramList);
			}

			private String updateFieldByIndex(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
				paramList.add(0, parser.get(UrlKeys.TABLENAME.asString()));
    			paramList.add(1, parser.get(JsonRequestKeys.PRIMARY_KEY_NAME.asString()));
    			paramList.add(2, parser.get(JsonRequestKeys.PRIMARY_KEY_VALUE.asString()));
    			paramList.add(3, parser.get(JsonRequestKeys.COLUMN_INDEX.asString()));
    			paramList.add(4, parser.get(JsonRequestKeys.NEW_VALUE.asString()));

    			return getResponseJsonString(clientInfo, iotMethod, paramList);
			}
		}
		
		/**********************************************
		 * Http Delete Method Handler Class
		 **********************************************/
		private class HttpDeleteMethodHandler implements HTTPMethodHandler {
			@Override
			public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException {
				String json = chooseAction(clientInfo);
				try {
					processResponseMsg(json, HttpStatusCode.OK, clientInfo);
				} catch (IOException e) {
					throw new SQLException(e);
				}
			}

			@Override
			public String chooseAction(ClientInfo clientInfo) throws SQLException {
				if(null == parser.getDatabaseMethod()) {
					return createJsonString(DatabaseKeys.ERROR_MESSAGE, INVALID_HTTP_METHOD);
				} else {
					switch(parser.getDatabaseMethod()) {
						case DELETE_TABLE:
							return createTable(clientInfo, parser.getDatabaseMethod());
						case DELETE_ROW:
							return createRow(clientInfo, parser.getDatabaseMethod());
						default:
							return createJsonString(DatabaseKeys.ERROR_MESSAGE, INVALID_HTTP_METHOD);
					}
				}
			}
			private String createTable(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
            	paramList.add(0, parser.get(UrlKeys.TABLENAME.asString()));
            	
            	return getResponseJsonString(clientInfo, iotMethod, paramList);
			}
			private String createRow(ClientInfo clientInfo, DatabaseKeys iotMethod) {
				List<Object> paramList = new ArrayList<>();
				paramList.add(0, parser.get(UrlKeys.TABLENAME.asString()));
    			paramList.add(1, parser.get(JsonRequestKeys.PRIMARY_KEY_NAME.asString()));
    			paramList.add(2, parser.get(JsonRequestKeys.PRIMARY_KEY_VALUE.asString()));

    			return getResponseJsonString(clientInfo, iotMethod, paramList);
			}
		}
		
		/**********************************************
		 * Http Options Method Handler Class
		 **********************************************/
		private class HttpOptionsMethodHandler implements HTTPMethodHandler {
			@Override
			public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException {
				String json = "";
				String response = HttpBuilder.createHttpResponseMessage(HttpVersion.HTTP_1_1,
																		HttpStatusCode.OK,
																		createOptionsHeader(json.length()),
																		json);

				ByteBuffer buffer = ByteBuffer.wrap(response.getBytes(Charset.forName("UTF-8")));
				try {
					clientInfo.tcpPath.write(buffer);
				} catch (IOException e) {
					throw new SQLException(e);
				}
			}

			@Override
			public String chooseAction(ClientInfo clientInfo) throws SQLException {
				return null;
			}
		}
		
		/**********************************************
		 * Http Trace Method Handler Class
		 **********************************************/
		private class HttpTraceMethodHandler implements HTTPMethodHandler {
			@Override
			public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException {
				try {
					sendNotImplementedResponse(clientInfo);
				} catch (IOException e) {
					throw new SQLException(e);
				}
			}

			@Override
			public String chooseAction(ClientInfo clientInfo) throws SQLException {
				return null;
			}
		}
		
		/**********************************************
		 * Http Patch Method Handler Class
		 **********************************************/
		private class HttpPatchMethodHandler implements HTTPMethodHandler {
			@Override
			public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException {
				try {
					sendNotImplementedResponse(clientInfo);
				} catch (IOException e) {
					throw new SQLException(e);
				}
			}

			@Override
			public String chooseAction(ClientInfo clientInfo) throws SQLException {
				return null;
			}
		}
		
		/**********************************************
		 * Http Head Method Handler Class
		 **********************************************/
		private class HttpHeadMethodHandler implements HTTPMethodHandler {
			@Override
			public void handleMessage(ClientInfo clientInfo, HTTPMessage httpMessage) throws SQLException {
				try {
					sendNotImplementedResponse(clientInfo);
				} catch (IOException e) {
					throw new SQLException(e);
				}
			}

			@Override
			public String chooseAction(ClientInfo clientInfo) throws SQLException {
				return null;
			}
		}
		
		/**********************************************
		 * Http Protocol implementation methods
		 **********************************************/
		
		private void sendNotImplementedResponse(ClientInfo clientInfo) throws IOException {
			String json = createJsonString(DatabaseKeys.ERROR_MESSAGE, NOT_IMPLEMENTED);
			sendMessage(HttpStatusCode.NOT_IMPLEMENTED, json, clientInfo);
		}

		private String getResponseJsonString(ClientInfo clientInfo, DatabaseKeys iotMethod, List<Object> paramList) {
			if (paramList.isEmpty()) {
				return createJsonString(DatabaseKeys.ERROR_MESSAGE, MISSING_PARAMS);
			}
			
			DatabaseManagementMessage replyToClient;
			try {
				replyToClient = iotMethodsMap.get(iotMethod).apply(clientInfo, parser.get(UrlKeys.DATABASE_NAME.asString()) ,paramList);
			} catch (IndexOutOfBoundsException | ClassCastException e) {
				replyToClient = createErrorMessage(parser.get(UrlKeys.DATABASE_NAME.asString()), e);
			}

			return createJsonString(iotMethod, replyToClient);
		}
		
		private String createJsonString(DatabaseKeys iotMethod, DatabaseManagementMessage replyToClient) {
			String json = responsesMap.get(replyToClient.getKey().getActionType()).getResponse();			
			JSONArray array = new JSONArray();
			for (Object object : replyToClient.getData()) {
				array.put(object.toString());
			}
			
			return completeResponse(json, array);
		}
		
		private String createJsonString(DatabaseKeys iotMethod, String message) {
			String json = responsesMap.get(iotMethod).getResponse();			
			JSONArray array = new JSONArray();
			array.put(message);

			return completeResponse(json, array);
		}
		
		private void processResponseMsg(String json, HttpStatusCode onSuccess, ClientInfo clientInfo) throws IOException {
			if (json.contains(INVALID_HTTP_METHOD)) {
				sendMessage(HttpStatusCode.BAD_REQUEST, json, clientInfo);
			} else {
				sendMessage(onSuccess, json, clientInfo);
			}			
		}
		
		private void sendMessage(HttpStatusCode statusCode, String json, ClientInfo clientInfo) throws IOException {
			String response = HttpBuilder.createHttpResponseMessage(parser.getVersion(),
																	statusCode,
																	createHeader(json.length()),
																	json);

			ByteBuffer buffer = ByteBuffer.wrap(response.getBytes(Charset.forName("UTF-8")));
			clientInfo.tcpPath.write(buffer);
		}
	
		private String completeResponse(String response, JSONArray array) {
			if(1 == array.length()) {
				return response + "\"" + array.get(0) + "\"" + "}";
			}
			
			return response + array.toString() + "}";
		}
	}
	
	/**********************************************
	 * Chat Server Protocol
	 **********************************************/
//	class ChatClientInfo {
//		private String name;
//
//		public ChatClientInfo(String name) {
//			this.name = name;
//		}
//		
//		public String getName() {
//			return name;
//		}
//	}
//	
//	private interface ChatProtocolKeysHandler {
//		public ChatServerMessage apply(ChatServerMessage clientMessage, ClientInfo clientInfo);
//	}
//	
//	private class ChatProtocol<K, V> implements Protocol<K, V> {			
//		private Map<SocketChannel, ChatClientInfo> registeredClients = new HashMap<>();
//		private Map<ChatProtocolKeys, ChatProtocolKeysHandler> chatMethodsMap = new HashMap<>();
//		
//		public ChatProtocol() {
//			chatMethodsMap.put(ChatProtocolKeys.REGISTRATION_REQUEST, new RegistrationReqHandler());
//			chatMethodsMap.put(ChatProtocolKeys.REGISTRATION_ACK, new WrongKeyHandler());
//			chatMethodsMap.put(ChatProtocolKeys.REGISTRATION_REFUSE, new WrongKeyHandler());
//			chatMethodsMap.put(ChatProtocolKeys.MESSAGE, new ClientMessageHandler());
//			chatMethodsMap.put(ChatProtocolKeys.BROADCAST_MESSAGE, new WrongKeyHandler());
//			chatMethodsMap.put(ChatProtocolKeys.REMOVE_REQUEST, new RemoveReqHandler());
//			chatMethodsMap.put(ChatProtocolKeys.ERROR_MESSAGE, new WrongKeyHandler());
//		}
//		
//		class RegistrationReqHandler implements ChatProtocolKeysHandler {
//			@Override
//			public ChatServerMessage apply(ChatServerMessage clientMessage, ClientInfo clientInfo) {
//				if(true == checkIfNameIsTaken(clientMessage.getData())) {
//					return new ChatServerMessage(ChatProtocolKeys.REGISTRATION_REFUSE, "Client with such name already exists");
//				}
//				
//				if(true == checkIfSocketIsTaken(clientInfo.tcpPath)) {
//					return new ChatServerMessage(ChatProtocolKeys.REGISTRATION_REFUSE, "Client with this socket already exists");
//				}
//				
//				try {
//					sendAllChatClients(new ChatServerMessage(ChatProtocolKeys.NEW_CLIENT_REGISTRATION, "New client registered: " + clientMessage.getData()));
//				} catch (IOException e) {
//					System.err.println("Couldn't send all clients the message");
//				}
//
//				registeredClients.put(clientInfo.tcpPath, new ChatClientInfo(clientMessage.getData()));
//				
//				return new ChatServerMessage(ChatProtocolKeys.REGISTRATION_ACK, clientMessage.getData() + " registered successfully");
//			}
//		}
//		
//		class WrongKeyHandler implements ChatProtocolKeysHandler {
//			@Override
//			public ChatServerMessage apply(ChatServerMessage clientMessage, ClientInfo clientInfo) {
//				return new ChatServerMessage(ChatProtocolKeys.ERROR_MESSAGE, "Wrong key");
//			}
//		}
//		
//		class RemoveReqHandler implements ChatProtocolKeysHandler {
//			@Override
//			public ChatServerMessage apply(ChatServerMessage clientMessage, ClientInfo clientInfo) {			
//				ChatServerMessage errorToClient = checkIfNoSuchClient(clientInfo);
//			
//				if(null == errorToClient) {
//					registeredClients.remove(clientInfo.tcpPath);
//					try {
//						sendAllChatClients(new ChatServerMessage(ChatProtocolKeys.MESSAGE, "Client left chat: " + clientMessage.getData()));
//					} catch (IOException e) {
//						System.err.println("Couldn't send all clients the message");
//					}
//				}
//				
//				return errorToClient;
//			}
//		}
//		
//		class ClientMessageHandler implements ChatProtocolKeysHandler {
//			@Override
//			public ChatServerMessage apply(ChatServerMessage clientMessage, ClientInfo clientInfo) {
//				ChatServerMessage errorToClient = checkIfNoSuchClient(clientInfo);
//
//				if(null == errorToClient) {				
//					try {
//						sendAllChatClientsExcept(new ChatServerMessage(ChatProtocolKeys.BROADCAST_MESSAGE,registeredClients.get(clientInfo.tcpPath) + ": " + clientMessage.getData()), clientInfo.tcpPath);
//					} catch (IOException e) {
//						System.err.println("Couldn't send all clients the message");
//					}
//				}
//				return errorToClient;
//			}
//		}
//	
//		@Override
//		public void handleMessage(ClientInfo clientInfo, Message<K, V>message) throws IOException {
//			ChatServerMessage chatMessage = (ChatServerMessage)message;
//			
//			ChatServerMessage replyToClient = chatMethodsMap.get(chatMessage.getKey()).apply(chatMessage, clientInfo);
//			
//			if(null != replyToClient) {
//				ServerMessage messageToSend = new ServerMessage(ProtocolType.CHAT_SERVER, replyToClient);
//				ByteBuffer buffer = ByteBuffer.wrap(BytesUtil.toByteArray(messageToSend));
//				clientInfo.tcpPath.write(buffer);
//			}
//		}
//	
//		private void sendAllChatClients(ChatServerMessage message) throws IOException {
//			ServerMessage messageToSend = new ServerMessage(ProtocolType.CHAT_SERVER, message);
//			ByteBuffer buffer = ByteBuffer.wrap(BytesUtil.toByteArray(messageToSend));
//			for (SocketChannel clientChannel : registeredClients.keySet()) {
//				clientChannel.write(buffer);
//				buffer.clear();
//			}
//		}
//		
//		private void sendAllChatClientsExcept(ChatServerMessage message, SocketChannel clientNotToSend) throws IOException {
//			ServerMessage messageToSend = new ServerMessage(ProtocolType.CHAT_SERVER, message);
//			ByteBuffer buffer = ByteBuffer.wrap(BytesUtil.toByteArray(messageToSend));
//			for (SocketChannel otherClientChannel : registeredClients.keySet()) {
//				if(!otherClientChannel.equals(clientNotToSend)) {					
//					otherClientChannel.write(buffer);
//					buffer.clear();
//				}
//			}
//		}
//		
//		private boolean checkIfNameIsTaken(String name) {
//			for (ChatClientInfo clientInfo : registeredClients.values()) {
//				if(clientInfo.getName().equals(name)) {
//					return true;
//				}
//			}
//			return false;
//		}
//		
//		private boolean checkIfSocketIsTaken(SocketChannel clientSocket) {
//			if(null != registeredClients.get(clientSocket)) {
//				return true;
//			}
//			return false;
//		}
//		
//		private ChatServerMessage checkIfNoSuchClient(ClientInfo clientInfo) {
//			if(null == registeredClients.get(clientInfo.tcpPath)) {
//				return new ChatServerMessage(ChatProtocolKeys.ERROR_MESSAGE, "You aren't registered");
//			}
//			return null;
//		}
//	}
	
	/***********************************************
	 * Message Handler
	 **********************************************/
	private class MessageHandler {
		private Map<ProtocolType, Protocol> protocolMap = new HashMap<>();
		
		public MessageHandler() {
//			addProtocol(ProtocolType.PINGPONG, new PingPongProtocol<>());
//			addProtocol(ProtocolType.DATABASE_MANAGEMENT, new ChatProtocol<>());
//			addProtocol(ProtocolType.DATABASE_MANAGEMENT, new IOTProtocol<>());
			addProtocol(ProtocolType.DB_HTTP, new HttpProtocol<>());
		}

		private void handleMessage(ByteBuffer message, ClientInfo currentConnection) throws ClassNotFoundException, IOException {
			ServerMessage clientMessage = (ServerMessage) BytesUtil.toObject(message.array());
			ProtocolType index = clientMessage.getKey();

			protocolMap.get(index).handleMessage(currentConnection, clientMessage.getData());
		}

		private void addProtocol(ProtocolType index, Protocol protocol) {
			protocolMap.put(index, protocol);
		}

		private void removeProtocol(Protocol protocol) {
			protocolMap.remove(protocol);
		}
	}

	public void fillParamListAccordingToHttpMethod(List<Object> paramList, HttpMethod httpMethod) {
		// TODO Auto-generated method stub
		
	}
}