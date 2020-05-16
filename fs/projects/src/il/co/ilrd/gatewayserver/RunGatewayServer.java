package il.co.ilrd.gatewayserver;

public class RunGatewayServer {
	public static void main(String[] args) {
		try {
			GatewayServer gatewayServer = new GatewayServer();
			gatewayServer.addTcpServer(ServerPort.TCP_SERVER_PORT.getPort());
			gatewayServer.addUdpServer(ServerPort.UDP_SERVER_PORT.getPort());
			gatewayServer.addLowLevelHttpServer(ServerPort.HTTP_SERVER_PORT.getPort());
			//gatewayServer.addHighLevelHttpServer(ServerPort.HTTP_SERVER_PORT.getPort());
			gatewayServer.start();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}