package il.co.ilrd.raspi_clients;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;

import org.json.JSONObject;

public class Utils {
	final static String COMPANY_NAME = "Amazing.Inc";
	
	final static String COMMAND_KEY = "Commandkey";
	final static String IOT_UPDATE = "IOT_UPDATE";
	final static String DATA = "Data";
	final static String DB_NAME = "Amazing.Inc";
	final static String RAW_DATA = "rawData";
	final static String NULL = "NULL";

	public static JSONObject createMessage(String serialNumber, String description, String timeStamp) {
		if(null == timeStamp) {
			timeStamp = NULL;
		}
		
		String dataFromIotClient =	"'" + serialNumber  + "'" + "|" +
									"'" + description  + "'" + "|" +
									"'" + timeStamp  + "'";
		JSONObject innerJson = new JSONObject();
		innerJson.put(DB_NAME, COMPANY_NAME);
		innerJson.put(RAW_DATA, dataFromIotClient);
		
		JSONObject outerJson = new JSONObject();
		outerJson.put(COMMAND_KEY, IOT_UPDATE);
		outerJson.put(DATA, innerJson);
		
		System.out.println("return json will be:\n" + outerJson.toString());
		
		return outerJson;
	}
	
	public static DatagramPacket sendUDPMessage(DatagramSocket socket, JSONObject message, int portNumber, String ip) throws IOException {
			byte[] buffer = message.toString().getBytes();
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length, new InetSocketAddress(ip, portNumber));	
			socket.send(packet);
//			socket.receive(packet);			
//			String receivedMessage = new String(packet.getData(), 0, packet.getLength());
//			System.out.println("UDP client recieved: " + receivedMessage);
//			Thread.sleep(2000);
			return packet;

	}
	
	public static void receiveUDPMessage(DatagramSocket socket, DatagramPacket packet, JSONObject message, int portNumber, String ip) throws IOException {
		socket.receive(packet);			
		String receivedMessage = new String(packet.getData(), 0, packet.getLength());
		System.out.println("UDP client recieved: " + receivedMessage);
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
//		try (DatagramSocket socket = new DatagramSocket()){
//			byte[] buffer = message.toString().getBytes();
//			DatagramPacket packet = new DatagramPacket(buffer, buffer.length, new InetSocketAddress(ip, portNumber));	
//			socket.send(packet);
//			socket.receive(packet);			
//			String receivedMessage = new String(packet.getData(), 0, packet.getLength());
//			System.out.println("UDP client recieved: " + receivedMessage);
//			Thread.sleep(2000);
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
	}
}