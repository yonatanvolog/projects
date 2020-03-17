package il.co.ilrd.filedatabase;

import java.io.File;

import il.co.ilrd.observer.Callback;


public class ChangesAnalyzer {
	private File watchedFile;
	private File backupFile;
	private Callback<?> callback;
	private CRUDFile crudFile;
	
	public ChangesAnalyzer(String watchedFile, String backupFile) {
		// TODO Auto-generated constructor stub
	}
	
	public void register(FileWatcher<?> fileWatcher) {
		
	}
	
	private void compareFiles() {
		
	}
}