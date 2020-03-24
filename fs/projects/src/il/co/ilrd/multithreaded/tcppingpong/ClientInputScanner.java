package il.co.ilrd.multithreaded.tcppingpong;

import java.io.Closeable;
import java.io.IOException;
import java.util.Scanner;

public class ClientInputScanner extends Thread {
	Closeable socket;
	
	public ClientInputScanner(Closeable socket) {
		this.socket = socket;
	}
	
	@Override
	public void run() {
		System.out.println("Awaiting user input");
		Scanner scan = new Scanner(System.in);
		String myLine = scan.nextLine();
		if(myLine.contentEquals("exit")) {
			try {
				System.err.println("should have closed socket");
				socket.close();
				scan.close();
			} catch (IOException e) {
				// couldnt close socket, cannot handle
			}
		}
	}
}