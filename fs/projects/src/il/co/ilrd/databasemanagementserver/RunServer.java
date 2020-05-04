package il.co.ilrd.databasemanagementserver;

import java.io.IOException;
import static il.co.ilrd.pingpong.handlers.RunClients.*;

public class RunServer {	
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
		IOTServer server = new IOTServer();
			
//		server.addTcpConnection(ProtocolPort.CHAT_PROTOCOL_PORT.getPort());			
//		server.addTcpConnection(ProtocolPort.PINGPONG_PROTOCOL_PORT.getPort());
		server.addTcpConnection(ProtocolPort.DATABASE_MANAGEMENT_PORT.getPort());
	
		
		server.startServer();
	}
}
