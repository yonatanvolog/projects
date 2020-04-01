package il.co.ilrd.pingpong.handlers;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class EyalUDP {

	public static void main(String[] args) throws Exception {
		int portNumber = 50000;
		
		DatagramPacket packet = null;
		PingPongMessage message = new PingPongMessage(0, "Ping1");
		
		try (DatagramSocket socket = new DatagramSocket()){
			byte[] buffer = BytesUtil.toByteArray(message);
			packet = new DatagramPacket(buffer, buffer.length, InetAddress.getLocalHost(), portNumber);
			socket.send(packet);
			socket.receive(packet);

			String msg = (String) BytesUtil.toObject(packet.getData());
			System.out.println("EYAL? " + msg);
		}
    }
}
