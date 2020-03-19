package il.co.ilrd.filedatabase;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import java.util.Objects;

public class FileBackup {
	private String watchedFile = null;
	private String backupFile = null;
	private FileWatcherImpl fileWatcher;
	private ChangesAnalyzer changesAnalyzer = null;
	private static final String DEFAULT_BACKUP_FILE_PATH = "/home/yonatan/DefaultFileBackup.txt";
	
	public FileBackup(String watchedFile) throws IOException, InterruptedException {
		this.watchedFile = watchedFile;
		fileWatcher = new FileWatcherImpl(watchedFile);
	}
	
	public void setBackupFile(String backupFile) throws Exception {
		if(null != changesAnalyzer) {
			throw new Exception("Cannot set backup file after invoking startFileBackup() method");
		}
		this.backupFile = backupFile;
	}
	
	public void startFileBackup() throws Exception {
		if(null == backupFile) {
	        new File(DEFAULT_BACKUP_FILE_PATH).createNewFile();
			this.backupFile = DEFAULT_BACKUP_FILE_PATH;
		}
		changesAnalyzer = new ChangesAnalyzerImpl(this.watchedFile, backupFile);
		changesAnalyzer.register(fileWatcher);
	}
	
	public void stopFileBackup() throws IOException {
		fileWatcher.stopUpdate();
	}
	
	static int getNumOfLines(String filePath) throws IOException {
		Objects.requireNonNull(filePath);
		return (int)Files.lines(Paths.get(filePath)).count();
	}
	
	static void checkIfFileValid(File file) throws FileNotFoundException {
		if (!file.isFile()) {
			throw new FileNotFoundException("fileToWatch isn't a file");
		} else if (!file.exists()) {
			throw new NullPointerException("fileToWatch doesn't exist");
		}
	}	
}