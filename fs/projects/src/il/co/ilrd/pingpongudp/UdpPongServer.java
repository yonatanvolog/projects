package il.co.ilrd.pingpongudp;

import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class UdpPongServer {
	
	public static void main(String[] args) throws Exception {		
		System.out.println("Server: waiting for client");
				
		String Pong = "Pong!\n";
		DatagramSocket socket = new DatagramSocket(50000);
		DatagramPacket packetRecieved = new DatagramPacket(new byte[2000], 2000);
		DatagramPacket packetToSend = null;

	    for(int i = 5; i < 10; ++i) {
	    	socket.receive(packetRecieved);
	    	System.out.println("Server recieved: " + new String(packetRecieved.getData()));
	    	packetToSend =
					new DatagramPacket(Pong.getBytes(),Pong.length(),
									   packetRecieved.getAddress(), packetRecieved.getPort());
			socket.send(packetToSend);

	    }
	    socket.close();
	}
}
