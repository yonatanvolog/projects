package il.co.ilrd.filedatabase;

import java.nio.file.WatchEvent;

import il.co.ilrd.observer.Callback;

public abstract class ChangesAnalyzer {
	String watchedFile;
	String backupFile;
	Callback<WatchEvent<?>> callback;
	CRUD<Integer ,String> crudFile;
	
	public void register(FileWatcher fileWatcher) {
	}
}
