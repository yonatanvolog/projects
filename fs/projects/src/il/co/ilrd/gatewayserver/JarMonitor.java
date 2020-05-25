package il.co.ilrd.gatewayserver;

import static java.nio.file.StandardWatchEventKinds.ENTRY_CREATE;
import static java.nio.file.StandardWatchEventKinds.ENTRY_DELETE;
import static java.nio.file.StandardWatchEventKinds.ENTRY_MODIFY;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.ClosedWatchServiceException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.Objects;

import il.co.ilrd.observer.Callback;
import il.co.ilrd.observer.Dispatcher;

// ***** TODO : pass the class inside gatewayServer and change modifier of class to private !!!!  ******

public class JarMonitor implements DirMonitor {
	private final String JAR_EXTENSION = ".jar";
	private Dispatcher<String> dispatcher;
	private WatchService watcher;
	private File folderToWatch;
	private boolean keepWatching;
	private String dirPath;

	public JarMonitor(String dirPath) throws IOException {
		this.dirPath = dirPath;
		folderToWatch = new File(dirPath);
		checkIfFolderExists(folderToWatch);
		dispatcher = new Dispatcher<>();
		watcher = FileSystems.getDefault().newWatchService();;
		keepWatching = true;
		new WatcherThread(folderToWatch).start();
	}
	
	@Override
	public void register(Callback<String> callback) {
		dispatcher.register(Objects.requireNonNull(callback));		
	}
	
	@Override
	public void unregister(Callback<String> callback) {
		dispatcher.unregister(Objects.requireNonNull(callback));		
	}

	@Override
	public void stopUpdate() throws IOException {
		keepWatching = false;
		watcher.close();
	}
	
	private void updateAll(WatchEvent<?> event) {
		System.out.println("JarMonitor noticed change: \n" + dirPath + event.context().toString());
		dispatcher.updateAll(dirPath + event.context().toString());
	}
	
	private	class WatcherThread extends Thread {
		public WatcherThread(File fileName) throws IOException {
			Objects.requireNonNull(fileName);
			Path directory = fileName.toPath();
			directory.register(watcher, ENTRY_DELETE, 
										ENTRY_MODIFY);
		}
		
		@Override
		public void run() {
			while (keepWatching) {
				WatchKey eventsKey = null;
				try {
					eventsKey = watcher.take();
				} catch (InterruptedException e) {
					e.printStackTrace();
				} catch (ClosedWatchServiceException e){
					//continue
				}
				if (null != eventsKey) {
					checkEvents(eventsKey);					
				}
			}
		}
	}
	
	private void checkEvents(WatchKey eventsKey) {
		for (WatchEvent<?> event : eventsKey.pollEvents()) {
			final Path changedFile = (Path)event.context();
			if (changedFile.toString().endsWith(JAR_EXTENSION)) {
				updateAll(event);
			}
		}
		keepWatching = eventsKey.reset();
	}
	
	static void checkIfFolderExists(File file) throws FileNotFoundException {
		if (!file.exists()) {
			throw new NullPointerException("Jar monitor received a folder that doesn't exist");
		}
	}	
}
