package il.co.ilrd.pingpongtcp;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class TcpPongClient {

	public static void main(String[] args) throws Exception {
		int portNumber = 4444;
		
		Socket socket = new Socket("", portNumber);

        BufferedReader bufferedReader = 
        		new BufferedReader(new InputStreamReader(socket.getInputStream()));
        PrintWriter printWriter = new PrintWriter(socket.getOutputStream());

	    for(int i = 0; i < 5; ++i) {
	        printWriter.write(i + "Ping1!\n");
	        printWriter.flush();	
	        System.out.println("Client recieved: " + bufferedReader.readLine());
	    }
	    
	    bufferedReader.close();
        printWriter.close();
        socket.close();   
	}
}
