package il.co.ilrd.multithreaded.tcpudppingpong;

import java.io.Closeable;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import il.co.ilrd.multithreaded.tcppingpong.TcpPongMultiThreaded;

public class TcpUdpPongMultiThreaded {
	public static final int TCP_PORT = 50000;
	public static final int UDP_PORT = 60000;
	public static final int BROADCAST_PORT = 61000;

	public static void main(String[] args) throws IOException {
		TcpPongMultiThreaded tcpServer = new TcpPongMultiThreaded(TCP_PORT);
		UdpPongMultiThreaded udpServer = new UdpPongMultiThreaded(UDP_PORT);
		UdpPongMultiThreaded broadcastServer = new UdpPongMultiThreaded(BROADCAST_PORT);
		tcpServer.start();
		udpServer.start();
		broadcastServer.start();
		
		List<Closeable> socketList = new ArrayList<>();

		socketList.add(tcpServer);
		socketList.add(udpServer.getSocket());
		socketList.add(broadcastServer.getSocket());
	
		new ServerInputScanner(socketList).start();
	}
}

class UdpPongMultiThreaded extends Thread {
	private DatagramSocket datagramSocket;
	
	public UdpPongMultiThreaded(int portNumber) throws SocketException {
		datagramSocket = new DatagramSocket(portNumber);
	}
	
	public Closeable getSocket() {
		return datagramSocket;
	}
	
	
	@Override
	public void run() {
		boolean toContinue = true;
		System.out.println("UDPServer: waiting for client");
		DatagramPacket packetRecieved = null;
		DatagramPacket packetToSend = null;
		try {
				String Pong = "Pong from UDP Server!\n";
				
				packetRecieved = new DatagramPacket(new byte[2000], 2000);
				packetToSend = new DatagramPacket(new byte[2000], 2000);
				packetToSend.setData(Pong.getBytes());
				packetToSend.setLength(Pong.length() * 2);
		} catch (Exception e) {
			//failed to create packets, cannot handle
		}
		
		try {
			while(toContinue) {
				datagramSocket.receive(packetRecieved);
				System.out.println("UDP Server recieved: " + new String(packetRecieved.getData()));
				packetToSend.setAddress(packetRecieved.getAddress());
				packetToSend.setPort(packetRecieved.getPort());
				datagramSocket.send(packetToSend);
			}
		} catch (IOException e) {
			toContinue = false;
			System.out.println("UDP Server socket got closed");
		}
	}
}

class ServerInputScanner extends Thread {
	private List<Closeable> socketList;
	
	public ServerInputScanner(List<Closeable> socketList) {
		this.socketList = socketList;
	}
	
	@Override
	public void run() {
		System.out.println("Awaiting user input");
		Scanner scan = new Scanner(System.in);
		String myLine = scan.nextLine();
		if(myLine.contentEquals("exit")) {
			for (Closeable closeable : socketList) {
				try {
					closeable.close();
				} catch (IOException e) {
					//couldn't close socket, cannot handle
				}
			}
			scan.close();
		}
	}
}