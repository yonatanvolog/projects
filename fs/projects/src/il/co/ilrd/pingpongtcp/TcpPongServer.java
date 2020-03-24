package il.co.ilrd.pingpongtcp;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class TcpPongServer {

	public static void main(String[] args) throws Exception {
		int portNumber = 4444;
		
		System.out.println("Server: waiting for client");
		ServerSocket serverSocket = new ServerSocket(portNumber);
	    Socket socket = serverSocket.accept();
	    System.out.println("Server: Client connected");
	    
	    BufferedReader bufferedReader = 
	    		new BufferedReader(new InputStreamReader(socket.getInputStream()));
        PrintWriter printWriter = new PrintWriter(socket.getOutputStream());

	    for(int i = 5; i < 10; ++i) {
	        printWriter.write(i + "Pong!\n");
	        printWriter.flush();
	        System.out.println("Server recieved: " + bufferedReader.readLine());
	    }
	    
	    bufferedReader.close();
	    serverSocket.close();
        socket.close();
	}
}
