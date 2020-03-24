package il.co.ilrd.multithreaded.tcpudppingpong;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import il.co.ilrd.multithreaded.tcppingpong.ClientInputScanner;
import static il.co.ilrd.multithreaded.tcpudppingpong.TcpUdpPongMultiThreaded.*;

public class UdpPingClient1 {
	public static void main(String[] args) throws UnknownHostException, SocketException, InterruptedException {
		int portNumber = UDP_PORT;

		String Ping = "Ping from UDP client1!\n";
		InetAddress localHostAddress = InetAddress.getByName("");
		DatagramSocket socket = new DatagramSocket();
		new ClientInputScanner(socket).start();

		DatagramPacket packetRecieved = new DatagramPacket(new byte[2000], 2000);
		try {
			while(true) {
				DatagramPacket packetToSend = 
		   				new DatagramPacket(Ping.getBytes(), Ping.length(),
		   									localHostAddress, portNumber);
				socket.send(packetToSend);
				socket.receive(packetRecieved);
				System.out.println("Client recieved: " + new String(packetRecieved.getData()));
				Thread.sleep(3000);
			}
		} catch (IOException e) {
			System.err.println("UDP closed it's own socket");
		}
	}
}