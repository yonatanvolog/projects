package il.co.ilrd.httpiotserver;

import java.io.*;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.List;

public class RunHttpClient {

	public static void main(String[] args) throws Exception {
		new RunHttpClient("tadiran").startClient();
	}

	private String companyName;
//	ServerMessage serverMessage = null;
	ByteBuffer buffer = ByteBuffer.allocate(4096);
	SocketChannel clientSocket = null;
//	ServerMessage receivedMssage = null;

	public RunHttpClient(String companyName) {
		//this.companyName = companyName;
	}

	public void startClient() throws Exception {
		int portNumber = ProtocolPort.DB_HTTP_PORT.getPort();
		System.out.println(companyName + " Connecting to DB server " + InetAddress.getLocalHost() + " in port " + portNumber);

		try {
			clientSocket = SocketChannel.open(new InetSocketAddress(InetAddress.getLocalHost(), portNumber));
			//clientSocket = SocketChannel.open(new InetSocketAddress("172.20.20.3", portNumber));
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		//receieveMessageFromServer();


		sendHttpMessage("Yoni sends his regards");
	}
	
		
//		// send create DB message
//		prepareAndSendMessage(DatabaseKeys.CREATE_COMPANY_DATABASE, 
//				"CREATE TABLE TestCountry (country_id int AUTO_INCREMENT PRIMARY KEY, country_name varchar(20) NOT NULL UNIQUE)");
//
//		// invalid key
//		prepareAndSendMessage(DatabaseKeys.ACK_MESSAGE, 
//					  "CREATE TABLE TestCountry (country_id int AUTO_INCREMENT PRIMARY KEY, country_name varchar(20) NOT NULL UNIQUE)");
//
//		// send create table message
//		prepareAndSendMessage(DatabaseKeys.CREATE_TABLE, 
//				  "CREATE TABLE TestCountry (country_id int AUTO_INCREMENT PRIMARY KEY, country_name varchar(20) NOT NULL UNIQUE)");
//		// send create row message
//		prepareAndSendMessage(DatabaseKeys.CREATE_ROW,
//				"INSERT INTO TestCountry VALUES (NULL, 'England'),(NULL, 'Jordan')");
//		
//		// send read row message
//		prepareAndSendMessage(DatabaseKeys.READ_ROW, "TestCountry", "country_id", 1);
//		
//		// send read filed by index message
//		prepareAndSendMessage(DatabaseKeys.READ_FIELD_BY_INDEX, "TestCountry",  "country_id", 2, 2);
//					
//		// send create iot message //PROBLEM
//		prepareAndSendMessage(DatabaseKeys.CREATE_IOT_EVENT, "\"00001\"|\"software update 2.0\"|null");
//
//		// send read and update field by index message
//		prepareAndSendMessage(DatabaseKeys.READ_FIELD_BY_INDEX, "IOTEvent", "iot_event_id", 2, 3);
//
//		prepareAndSendMessage(DatabaseKeys.UPDATE_FIELD_BY_INDEX, "IOTEvent", "iot_event_id", 1, 3, "WORKING !!!!!");
//		prepareAndSendMessage(DatabaseKeys.READ_FIELD_BY_INDEX, "IOTEvent", "iot_event_id", 1, 3);
//
//		// send read and update field by Column name message
//		prepareAndSendMessage(DatabaseKeys.UPDATE_FIELD_BY_NAME, "IOTEvent", "iot_event_id", 1, "description",
//				"'new description :)'");
//		prepareAndSendMessage(DatabaseKeys.READ_FIELD_BY_NAME, "IOTEvent", "iot_event_id", 1, "description");
//
//		// send delete row
//		prepareAndSendMessage(DatabaseKeys.DELETE_ROW, "IOTEvent", "iot_event_id", 1);
//
//		// send delete table
//		prepareAndSendMessage(DatabaseKeys.DELETE_TABLE, "TestCountry");
//
//	}
//
//	private void prepareAndSendMessage(DatabaseKeys databaseKey, Object... objects) throws Exception {
//		List<Object> params = new ArrayList<>();
//		addToParamsList(params, objects);
//		sendMessage(new DatabaseManagementMessage(new ActionTypeKey(companyName, databaseKey), params));
//		Thread.sleep(2000);
//	}

//	private void receieveMessageFromServer() throws IOException, ClassNotFoundException {
//		new Thread(new Runnable() {
//
//			@Override
//			public void run() {
//				ByteBuffer receiveBuffer = ByteBuffer.allocate(4096);
//				while (true) {
//					receiveBuffer.clear();
//					try {
//						clientSocket.read(receiveBuffer);
//					} catch (IOException e) {
//						// TODO Auto-generated catch block
//						e.printStackTrace();
//					}
//					try {
//						receivedMssage = (ServerMessage) BytesUtil.toObject(receiveBuffer.array());
//					} catch (ClassNotFoundException e) {
//						// TODO Auto-generated catch block
//						e.printStackTrace();
//					} catch (IOException e) {
//						// TODO Auto-generated catch block
//						e.printStackTrace();
//					}
//					DatabaseManagementMessage dBreceivedMessage = (DatabaseManagementMessage) receivedMssage.getData();
//
//					System.out.println("\n" + "response:   " + dBreceivedMessage.getKey().getDatabaseName()
//							+ "   " + dBreceivedMessage.getKey().getActionType());
//					List<Object> response = dBreceivedMessage.getData();
//					
//					String printout = "description: ";
//					for (Object object : response) {
//						System.out.println(printout + object);
//
//					}
//				}
//			}
//		}).start();
//		;
//	}

	private void addToParamsList(List<Object> params, Object... objects) {
		params.clear();
		for (Object obj : objects) {
			params.add(obj);
		}
	}

//	private void sendMessage(DatabaseManagementMessage dBMessage) throws IOException {
//		serverMessage = new ServerMessage(ProtocolType.DATABASE_MANAGEMENT, dBMessage);
//		byte[] serverMessageArray = BytesUtil.toByteArray(serverMessage);
//		buffer.clear();
//		buffer.put(serverMessageArray);
//		buffer.flip();
//		clientSocket.write(buffer);
//	}
	
	private void sendHttpMessage(String httpMessage) throws IOException {
		//serverMessage = new ServerMessage(ProtocolType.DATABASE_MANAGEMENT, dBMessage);
		byte[] serverMessageArray = BytesUtil.toByteArray(httpMessage);
		buffer.clear();
		buffer.put(serverMessageArray);
		buffer.flip();
		clientSocket.write(buffer);
	}
}