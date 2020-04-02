package il.co.ilrd.pingpong.handlers;

import java.io.IOException;
import static il.co.ilrd.pingpong.handlers.RunClients.*;

public class RunServer {	
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {

		Server server = new Server();
		server.addTcpConnection(TCP_PORT1);
		server.addTcpConnection(TCP_PORT2);

		server.addUdpConnection(UDP_PORT1);
		server.addUdpConnection(UDP_PORT2);

		server.addBroadcastConnection(BROADCAST_PORT1);
		server.addBroadcastConnection(BROADCAST_PORT2);

		server.startServer();
		Thread.sleep(10000);
		server.stopServer();
	}
}
