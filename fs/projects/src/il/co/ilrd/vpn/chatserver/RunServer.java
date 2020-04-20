package il.co.ilrd.vpn.chatserver;

import java.io.IOException;
import static il.co.ilrd.pingpong.handlers.RunClients.*;

public class RunServer {	
	public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
		GlobalServer server = new GlobalServer();
			
		server.addTcpConnection(GlobalProtocolPort.CHAT_PROTOCOL_PORT.getPort());	
		server.addTcpConnection(GlobalProtocolPort.PINGPONG_PROTOCOL_PORT.getPort());			

		//server.addUdpConnection(GlobalProtocolPort.PINGPONG_PROTOCOL_PORT.getPort());

		server.startServer();
	}
}
