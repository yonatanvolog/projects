package il.co.ilrd.filedatabase;

import java.io.File;

import il.co.ilrd.observer.Callback;


public class ChangesAnalyzer {
	private File watchedFile;
	private File backupFile;
	private Callback<?> callback;
	private CRUDFile crudFile;
	
	public ChangesAnalyzer(String watchedFile, String backupFile) {
		this.watchedFile = new File(watchedFile);
		this.backupFile = new File(backupFile);
	}
	
	//subscribes itself to FileWatcher (to be notified - 
	//invoked it's compareFiles on notification)

	public void register(FileWatcher<?> fileWatcher) {
		
	}
	
	private void compareFiles() {
		
	}
}