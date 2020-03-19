package il.co.ilrd.filedatabase;

import java.nio.file.*;
import java.util.Objects;
import static java.nio.file.StandardWatchEventKinds.*;
import java.io.File;
import java.io.IOException;

import static il.co.ilrd.filedatabase.FileBackup.*;
import il.co.ilrd.observer.*;

public class FileWatcherImpl extends FileWatcher {
	private Dispatcher<WatchEvent<?>> dispatcher;
	private WatchService watcher;
	private boolean keepWatching;
	
	public FileWatcherImpl(String fileToWatchPath) throws IOException, InterruptedException {
		fileToWatch = new File(fileToWatchPath);
		checkIfFileValid(fileToWatch);
		dispatcher = new Dispatcher<WatchEvent<?>>();
		watcher = FileSystems.getDefault().newWatchService();
		keepWatching = true;
		new WatcherThread(fileToWatch.getParentFile()).start();
	}
	
	@Override
	public void register(Callback<WatchEvent<?>> callback) {
		Objects.requireNonNull(callback);
		dispatcher.register(callback);
	}

	@Override
	public void unregister(Callback<WatchEvent<?>> callback) {
		Objects.requireNonNull(callback);
		dispatcher.unregister(callback);
	}

	@Override
	public void stopUpdate() throws IOException {
		keepWatching = false;
		watcher.close();
	}

	private void updateAll(WatchEvent<?> event) {
		dispatcher.updateAll(event);
	}
	
	private	class WatcherThread extends Thread {
		public WatcherThread(File fileName) throws IOException {
			Objects.requireNonNull(fileName);
			Path directory = fileName.toPath();
			directory.register(watcher, ENTRY_CREATE, 
										ENTRY_DELETE, 
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
		Objects.requireNonNull(eventsKey);
		for (WatchEvent<?> event : eventsKey.pollEvents()) {
			final Path changedFile = (Path)event.context();
			if (changedFile.endsWith(fileToWatch.getName())) {
				updateAll(event);
			}
		}
		keepWatching = eventsKey.reset();
	}
}