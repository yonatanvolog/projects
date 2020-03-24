package il.co.ilrd.pingpongtcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;

public class DownloadWebpage {
	public static void DownloadWebPage(String webpage) {
		try {
			URL url = new URL(webpage);
			BufferedReader br = new BufferedReader(new InputStreamReader(url.openStream())); 
			String line = null;
			while((line = br.readLine()) != null) {
				System.out.println(line);
			}
			br.close();
			System.out.println("Finished download");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		DownloadWebPage("https://www.geeksforgeeks.org/");
	}
}
