package il.co.ilrd.sunhttpserver;

import java.io.IOException;
import static il.co.ilrd.pingpong.handlers.RunClients.*;

public class RunSunServer {	
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
		Server server = new Server();
			
//		server.addTcpConnection(ProtocolPort.CHAT_PROTOCOL_PORT.getPort());			
//		server.addTcpConnection(ProtocolPort.PINGPONG_PROTOCOL_PORT.getPort());
//		server.addTcpConnection(ProtocolPort.DATABASE_MANAGEMENT_PORT.getPort());
		server.addSunHttpConnection(ProtocolPort.DB_HTTP_PORT.getPort());
		
		server.startServer();
	}
}
