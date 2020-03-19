package il.co.ilrd.filedatabase;

import java.io.File;
import java.io.IOException;
import java.nio.file.WatchEvent;
import java.nio.file.WatchService;

import il.co.ilrd.observer.Callback;
import il.co.ilrd.observer.Dispatcher;

public abstract class FileWatcher {
	File fileToWatch;

	public void register(Callback<WatchEvent<?>> callback) {
	}
	
	public void unregister(Callback<WatchEvent<?>> callback) {
	}
	
	public void stopUpdate() throws IOException {
	}
}