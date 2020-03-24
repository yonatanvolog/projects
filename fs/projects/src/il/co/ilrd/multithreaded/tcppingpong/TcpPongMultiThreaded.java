package il.co.ilrd.multithreaded.tcppingpong;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class TcpPongMultiThreaded extends Thread implements Closeable {
	private ServerSocket serverSocket;
	private List<Closeable> socketList;
	private boolean toContinue = true;
	
	public TcpPongMultiThreaded(int portNumber) throws IOException {
		serverSocket = new ServerSocket(portNumber);
		socketList = new ArrayList<>();
	}

	@Override
	public void close() throws IOException {
		toContinue = false;
		serverSocket.close();
		for (Closeable closeable : socketList) {
			try {
				closeable.close();
			} catch (IOException e) {
				// couldnt close socket, cannot handle
			}
		}
	}
	
	@Override
	public void run() {
		try {
			Socket socket = null;
			
			System.out.println("TCPMultiServer waiting for client");

			while(toContinue) {
				socket = serverSocket.accept();
				socketList.add(socket);
				new TCPSingleClientHandler(socket).start();
			}
			
		
		} catch (IOException e) {
			toContinue = false;
			System.out.println("TCPMultiServer socket got closed");
		}
	}
	
	class TCPSingleClientHandler extends Thread {
		private Socket socket;
		
		public TCPSingleClientHandler(Socket socket) {
			this.socket = socket;
		}
		
		public Closeable getSocket() {
			return serverSocket;
		}
		
		@Override
		public void run() {
			BufferedReader bufferedReader = null;
			PrintWriter printWriter = null;
			
			try {
				bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				printWriter = new PrintWriter(socket.getOutputStream());
	
			} catch (IOException e1) {
				// cannot handle
			}
			
			try {
				while(toContinue) {
					System.out.println("TCP Server recieved: " + bufferedReader.readLine());
					printWriter.write("Pong from TCP Server!\n");
					printWriter.flush();
					Thread.sleep(1000);
				}
			} catch (Exception e) {
				System.out.println("Client closed TCP socket");
			}
		}
	}
}