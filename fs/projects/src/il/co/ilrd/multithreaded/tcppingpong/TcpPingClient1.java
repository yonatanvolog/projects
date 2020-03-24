package il.co.ilrd.multithreaded.tcppingpong;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;

import il.co.ilrd.multithreaded.tcppingpong.ClientInputScanner;
import static il.co.ilrd.multithreaded.tcpudppingpong.TcpUdpPongMultiThreaded.*;

public class TcpPingClient1 {

	public static void main(String[] args) throws IOException, InterruptedException {		
		int portNumber = TCP_PORT;
		
		Socket socket = new Socket("", portNumber);
		new ClientInputScanner(socket).start();

        BufferedReader bufferedReader = 
        		new BufferedReader(new InputStreamReader(socket.getInputStream()));
        PrintWriter printWriter = new PrintWriter(socket.getOutputStream());

        String recieved = "";
        
	    while(null != recieved) {
	        try {
		        Thread.sleep(3000);
	        	printWriter.write("Ping from client1!\n");
		        printWriter.flush();
		        recieved = bufferedReader.readLine();
				System.out.println("Client recieved: " + recieved);

	        } catch (SocketException e) {
				System.err.println("TCPClient socket successfully closed!");
				return;
			}

	    }	    
	    bufferedReader.close();
        printWriter.close();
	}
}
