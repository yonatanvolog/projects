package il.co.ilrd.gatewayserver;

import java.io.IOException;

public class RunGatewayServer {
	public static void main(String[] args) {
		try {
			GatewayServer gatewayServer = new GatewayServer();
			gatewayServer.addTcpServer(ServerPort.TCP_SERVER_PORT.getPort());
			gatewayServer.start();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
