package il.co.ilrd.gatewayserver;

import java.io.*;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class RunTcpClient {

	public static void main(String[] args) throws Exception {
		new RunTcpClient().startClient();
	}

	private final String TEST_JSON = "{\"Commandkey\": \"COMPANY_REGISTRATION\",\r\n" + 
									 "	\"Data\": \"test message from TCP client\"\r\n" + 
									 "}\r\n";
	private ByteBuffer buffer = ByteBuffer.allocate(4096);
	private SocketChannel clientSocket = null;
	private String receivedMessage = null;


	public void startClient() throws Exception {
		int portNumber = ServerPort.TCP_SERVER_PORT.getPort();
		System.out.println("Connecting to server " + InetAddress.getLocalHost() + " in port " + portNumber);

		try {
			clientSocket = SocketChannel.open(new InetSocketAddress(InetAddress.getLocalHost(), portNumber));
			//clientSocket = SocketChannel.open(new InetSocketAddress("172.20.20.3", portNumber));
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		receieveMessageFromServer();
		sendMessage(TEST_JSON);
	}

	private void receieveMessageFromServer() throws IOException, ClassNotFoundException {
		new Thread(new Runnable() {

			@Override
			public void run() {
				ByteBuffer receiveBuffer = ByteBuffer.allocate(4096);
				while (true) {
					receiveBuffer.clear();
					try {
						clientSocket.read(receiveBuffer);
					} catch (IOException e) {
						e.printStackTrace();
					}
					try {
						receivedMessage = (String) BytesUtil.toObject(receiveBuffer.array());
					} catch (ClassNotFoundException | IOException e) {
						e.printStackTrace();
					}
					System.out.println("TCP client recieved" + receivedMessage);

				}
			}
		}).start();
		;
	}

	private void sendMessage(String message) throws IOException {
		byte[] serverMessageArray = BytesUtil.toByteArray(message);
		buffer.clear();
		buffer.put(serverMessageArray);
		buffer.flip();
		clientSocket.write(buffer);
	}
}