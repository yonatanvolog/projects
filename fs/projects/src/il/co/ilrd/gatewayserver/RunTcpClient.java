package il.co.ilrd.gatewayserver;import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.nio.charset.StandardCharsets;
import java.io.*;
import java.net.InetAddress;
import java.net.InetSocketAddress;

public class RunTcpClient {

	public static void main(String[] args) throws Exception {
		new RunTcpClient().startClient();
	}

	private final String TEST_JSON = "{\"Commandkey\": \"COMPANY_REGISTRATION\",\r\n" + 
									 "	\"Data\": \"TCP client data\"\r\n" + 
									 "}\r\n";
	private SocketChannel clientSocket = null;

	public void startClient() throws Exception {
		int portNumber = ServerPort.TCP_SERVER_PORT.getPort();
		System.out.println("Connecting to server " + InetAddress.getLocalHost() + " in port " + portNumber);

		try {
			clientSocket = SocketChannel.open(new InetSocketAddress(InetAddress.getLocalHost(), portNumber));
			//clientSocket = SocketChannel.open(new InetSocketAddress("172.20.20.3", portNumber));
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		sendMessage(TEST_JSON);
		receieveMessageFromServer();
	}

	private void receieveMessageFromServer() throws IOException, ClassNotFoundException {
		new Thread(new Runnable() {

			@Override
			public void run() {
				ByteBuffer receiveBuffer = ByteBuffer.allocate(4096);
				receiveBuffer.clear();  
				try {
					clientSocket.read(receiveBuffer);
				} catch (IOException e) {
					e.printStackTrace();
				}
				receiveBuffer.flip();
				String receivedMessage = bufferToString(receiveBuffer);
				System.out.println("TCP client recieved: " + receivedMessage);


			}
		}).start();
		;
	}

	private void sendMessage(String message) throws IOException {
		clientSocket.write(stringToBuffer(message));
	} 
	
	private String bufferToString(ByteBuffer dataBuffer) {
		byte[] arr = new byte[dataBuffer.limit()];
		dataBuffer.get(arr, 0, arr.length);
		dataBuffer.clear(); 
		
		return new String(arr, StandardCharsets.UTF_8);
	}
		
	private ByteBuffer stringToBuffer(String stringToProcess) {
		return ByteBuffer.wrap(stringToProcess.getBytes());
	}
}