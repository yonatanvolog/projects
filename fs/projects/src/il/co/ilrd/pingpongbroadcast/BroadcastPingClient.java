package il.co.ilrd.pingpongbroadcast;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class BroadcastPingClient {

	public static void main(String[] args) throws Exception {
		int portNumber = 50000;
		String broadcastAddress = "255.255.255.255";
		
		
		String Ping = "Ping!\n";
		InetAddress localHostAddress = InetAddress.getByName(broadcastAddress);
		DatagramSocket socket = new DatagramSocket();
		DatagramPacket packetToSend;
		DatagramPacket packetRecieved = new DatagramPacket(new byte[2000], 2000);
		
		socket.setBroadcast(true);
	    for(int i = 0; i < 5; ++i) {
	    	packetToSend = 
	    				new DatagramPacket(Ping.getBytes(), Ping.length(),
	    									localHostAddress, portNumber);
			socket.send(packetToSend);
			socket.receive(packetRecieved);
			System.out.println("Client recieved: " + new String(packetRecieved.getData()));

	    }
	    socket.close();
	}	

}
