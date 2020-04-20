package il.co.ilrd.vpn.chatserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class RunClientMy {
	private ByteBuffer buffer = ByteBuffer.allocate(2048); 
	private GlobalServerMessage message;
	private SocketChannel socket;
	private boolean toContinue = true;
	private boolean keepListening = true;
	private static final int PORT_NUM = GlobalProtocolPort.CHAT_PROTOCOL_PORT.getPort();
	private static final String host = "172.20.20.5";

	
	public static void main(String[] args) throws UnknownHostException, ClassNotFoundException, IOException {
		RunClientMy user = new RunClientMy();
		user.runCheckInputThread();
	}
	
	private void registerToChat(String userName) throws UnknownHostException, IOException, ClassNotFoundException {
		socket = SocketChannel.open(new InetSocketAddress(InetAddress.getByName(host), PORT_NUM));
		message = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, 
				new GlobalChatServerMessage(GlobalChatProtocolKeys.REGISTRATION_REQUEST, userName));
		buffer.put(BytesUtil.toByteArray(message));
		keepListening = true;
		runPrintMessages();
		buffer.clear();
		socket.write(buffer);
		buffer.clear();
	}
	
	private void removerFromChat(String userName) throws UnknownHostException, IOException, ClassNotFoundException {
		keepListening = false;
		message = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, 
				new GlobalChatServerMessage(GlobalChatProtocolKeys.REMOVE_REQUEST, userName));
		buffer.put(BytesUtil.toByteArray(message));
		buffer.clear();
		socket.write(buffer);
		buffer.clear();
	}
	
	private void sendMessage(String messageString) throws UnknownHostException, IOException, ClassNotFoundException {
		message = new GlobalServerMessage(GlobalProtocolType.CHAT_SERVER, 
				new GlobalChatServerMessage(GlobalChatProtocolKeys.MESSAGE, messageString));
		buffer.put(BytesUtil.toByteArray(message));
		buffer.clear();
		socket.write(buffer);
		buffer.clear();
	}
	
	private void runCheckInputThread() {
		
		Runnable stopRunnable = new Runnable() {
			public void run() {
				String dataString = "initString";
				String userName = null;
				
				try(BufferedReader input = new BufferedReader(new InputStreamReader(System.in))) {
					while(toContinue) {
						dataString = input.readLine(); 
						switch (dataString) {
							case "join":
								System.out.println("please enter the the user name:");
								userName = input.readLine();
								registerToChat(userName);
								break;
							case "remove":
								removerFromChat(userName);
								break;
							default:
								if(dataString.equals("exit")) {
									removerFromChat(userName);
									toContinue = false;
									break;
								}
								sendMessage(userName + ": " + dataString);
								break;
						}
					}
					
				} catch (IOException e){
					System.out.println("input thread: " + e);
				} catch (ClassNotFoundException e) {
					System.out.println("input thread: " + e);
				}
			}
		};
		new Thread(stopRunnable).start();
	}
	
	private void runPrintMessages() throws IOException, ClassNotFoundException {
		Runnable messagePrinting = new Runnable() {
			
			@Override
			public void run() {
				try {
					while(keepListening) {
						socket.read(buffer);
						GlobalServerMessage retMessage = (GlobalServerMessage) BytesUtil.toObject(buffer.array());
						System.out.println(retMessage.getData().getData());
						buffer.clear();			
					}									
				} catch (Exception e) {
					System.out.println("exception");
					if(keepListening) {
						throw new RuntimeException(e);
					}
				}
			}
		};
		new Thread(messagePrinting).start();
	}
}
