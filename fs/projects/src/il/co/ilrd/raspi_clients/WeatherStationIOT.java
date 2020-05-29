package il.co.ilrd.raspi_clients;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketException;
import java.util.Map;

import org.json.JSONObject;

import il.co.ilrd.gatewayserver.ServerPort;

public class WeatherStationIOT implements IOTDevice {
	private String serialNumber;
	private int portNumber;
	private String ip;
	
	private Battery battery;
	private Humiditysensor humiditySensor;
	private TempSensor tempSensor;
	
	private DatagramSocket socket;

	private WritingThread writingThread = new WritingThread();
	private ReadingThread readingThread;
	private Map<String, String> awaitingAck;
	
	private final static int BUFFER_SIZE = 1024*6;

	WeatherStationIOT ws = new WeatherStationIOT("MY_SERIAL", ServerPort.UDP_SERVER_PORT.getPort(), "10.1.0.72");
	
	private class WritingThread extends Thread {
		@Override
		public void run() {
			//fetch message from concurrent map
			byte[] buffer = "hello".toString().getBytes();
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length, new InetSocketAddress(ip, portNumber));	
			try {
				socket.send(packet);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	private class ReadingThread extends Thread {
		@Override
		public void run() {
			DatagramPacket packet = new DatagramPacket(new byte[BUFFER_SIZE], BUFFER_SIZE);	

			try {
				socket.receive(packet);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			String receivedMessage = new String(packet.getData(), 0, packet.getLength());
			System.out.println("UDP client recieved: " + receivedMessage);
			try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	
	WeatherStationIOT(String serialNumber, int portNumber, String ip) throws SocketException{
		this.serialNumber = serialNumber;
		this.portNumber = portNumber;
		this.ip = ip;
		
		battery = new Battery(100);
		battery.start();
		
		humiditySensor = new Humiditysensor();
		tempSensor = new TempSensor();
		
		socket = new DatagramSocket();
	}

	public static void main(String[] args) throws SocketException, InterruptedException {
		WeatherStationIOT ws = new WeatherStationIOT("MY_SERIAL", ServerPort.UDP_SERVER_PORT.getPort(), "10.1.0.72");
		while(true) {
			System.err.println("will send message in 3 seconds");
			Thread.sleep(3000);
			ws.sendUpdate();
		}
	}
	
	@Override
	public void sendUpdate() {
		String description = "temp: " + tempSensor.getTemp() + " ,humidity: " + humiditySensor.getHumidistLevel();
		JSONObject message = Utils.createMessage(serialNumber, description, null);
		
		
		try {
			DatagramPacket packet = Utils.sendUDPMessage(socket, message, portNumber, ip);
			Utils.receiveUDPMessage(socket, packet, message, portNumber, description);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void sendWarning() {
		// TODO Auto-generated method stub

	}

	@Override
	public void sendError() {
		// TODO Auto-generated method stub

	}

	@Override
	public void turnOnLight() {
		// TODO Auto-generated method stub

	}
}