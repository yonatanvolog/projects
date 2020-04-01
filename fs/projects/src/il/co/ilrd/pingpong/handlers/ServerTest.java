package il.co.ilrd.pingpong.handlers;

import java.io.IOException;

public class ServerTest {

	public static void main(String[] args) throws IOException, ClassNotFoundException {
		
		new Thread(new UdpPingClientMessage("Ping1")).start();
		//new Thread(new UdpPingClientMessage("Ping2")).start();

		
		Server server = new Server();
		server.addUdpConnection(50000);
		server.startServer();
		System.out.println("after start");
	}

}
