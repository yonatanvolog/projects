package il.co.ilrd.gatewayserver;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class RunUdpClient {
	public static void main(String[] args) throws Exception {
		new RunUdpClient().startClient();
	}
	
	private final String TEST_JSON = "{\"Commandkey\": \"PRODUCT_REGISTRATION\",\r\n" + 
									 "	\"Data\": \"UDP client data\"\r\n" + 
									 "}\r\n";
	
	public void startClient() throws ClassNotFoundException, IOException {
		int portNumber = ServerPort.UDP_SERVER_PORT.getPort();
		System.out.println("Connecting to server " + InetAddress.getLocalHost() + " in port " + portNumber);
		DatagramPacket packet = null;
		//receieveMessageFromServer();
		try (DatagramSocket socket = new DatagramSocket()){
			byte[] buffer = TEST_JSON.getBytes();
			packet = new DatagramPacket(buffer, buffer.length, InetAddress.getLocalHost(), portNumber);	
			socket.send(packet);
			socket.receive(packet);			
			String receivedMessage = new String(packet.getData(), 0, packet.getLength());
			System.out.println("UDP client recieved: " + receivedMessage);
			Thread.sleep(2000);
		} catch (Exception e) {
			e.printStackTrace();
		}	
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
//						e.printStackTrace();
//					}
//					try {
//						receivedMessage = (String) BytesUtil.toObject(receiveBuffer.array());
//					} catch (ClassNotFoundException | IOException e) {
//						e.printStackTrace();
//					}
//					System.out.println("TCP client recieved: " + receivedMessage);
//
//				}
//			}
//		}).start();
//		;
//	}
}