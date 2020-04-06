package il.co.ilrd.chatserver;

import java.io.IOException;
import static il.co.ilrd.pingpong.handlers.RunClients.*;

public class RunServer {	
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
		Server server = new Server();
			
		server.addTcpConnection(ProtocolPort.CHAT_PROTOCOL_PORT.getPort());			
		server.addUdpConnection(ProtocolPort.PINGPONG_PROTOCOL_PORT.getPort());

		server.startServer();
	}
}
