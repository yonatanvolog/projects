package il.co.ilrd.pingpongudp;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UdpPingClient {
	public static void main(String[] args) throws Exception {
		int portNumber = 50000;

		String Ping = "Ping!\n";
		InetAddress localHostAddress = InetAddress.getByName("yonatan-MS-7752");
		DatagramSocket socket = new DatagramSocket();
		DatagramPacket packetToSend;
		DatagramPacket packetRecieved = new DatagramPacket(new byte[2000], 2000);
				
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
