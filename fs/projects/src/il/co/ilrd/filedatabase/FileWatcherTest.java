package il.co.ilrd.filedatabase;

import java.io.IOException;
import java.nio.file.WatchEvent;
import java.util.function.Consumer;

import il.co.ilrd.observer.Callback;


public class FileWatcherTest {
	
	static void imUpdated() {
		System.out.println("my file has changed");
	}

	static void imStopUpdated() {
		System.out.println("I'm stopped being notified about file changes");
	}

	public static void main(String[] args) throws IOException, InterruptedException {
		FileWatcher fw = new FileWatcher("/home/yonatan/myTestFile.txt");
		//FileWatcher fw = new FileWatcher("/home/yonatan");

		Consumer myUpdateFunc = (T) -> imUpdated();
		Consumer myStopUpdateFunc = (T) -> imStopUpdated();	
		
		Callback<WatchEvent<?>> userCallback = new Callback(myUpdateFunc, myStopUpdateFunc);
		
		fw.register(userCallback);
		Thread.sleep(10000);

		System.out.println("after sleep");
		fw.stopUpdate();
		
		//fw.unregister(userCallback);
	}
}
