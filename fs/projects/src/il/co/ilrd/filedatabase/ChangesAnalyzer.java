package il.co.ilrd.filedatabase;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.WatchEvent;
import java.util.Objects;

import il.co.ilrd.observer.Callback;
import static il.co.ilrd.filedatabase.FileWatcher.checkIfFileValid;

public class ChangesAnalyzer {
	private String watchedFile;
	private String backupFile;
	private Callback<WatchEvent<?>> callback;
	private CRUDFile crudFile;
	int lastBackupedLine = 0;
	
	public ChangesAnalyzer(String watchedFile, String backupFile) throws IOException {
		Objects.requireNonNull(watchedFile);
		Objects.requireNonNull(backupFile);		
		checkIfFileValid(new File(watchedFile));
		checkIfFileValid(new File(watchedFile));
		
		this.watchedFile = watchedFile;
		this.backupFile = backupFile;
		
		callback = new Callback<>((T) -> {
			try {
				compareFiles();
			} catch (IOException e) {
				// cannot rethrow
			}
		} ,null);
		crudFile = new CRUDFile(backupFile);
	}

	public void register(FileWatcher fileWatcher) {
		Objects.requireNonNull(fileWatcher);
		fileWatcher.register(callback);
	}

	private void compareFiles() throws IOException {	
		int watchedFileLines = getNumOfLines(watchedFile);
		int backupFileLines = getNumOfLines(backupFile);

		if(watchedFileLines > backupFileLines) {			
			FileInputStream fileInputsStream = new FileInputStream(watchedFile);
			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(fileInputsStream));
			bringBufferedReaderToLine(bufferedReader, lastBackupedLine);
			copyLinesFromWatchedToBackup(bufferedReader, watchedFileLines - lastBackupedLine);
			lastBackupedLine = watchedFileLines;
		}
	}
	
	static int getNumOfLines(String filePath) throws IOException {
		Objects.requireNonNull(filePath);
		return (int)Files.lines(Paths.get(filePath)).count();
	}
	
	private void bringBufferedReaderToLine(BufferedReader bufferedReader, int lastBackupedLine) throws IOException {
		Objects.requireNonNull(bufferedReader);
		for(int i = 0; i < lastBackupedLine; ++i) {
			bufferedReader.readLine();
		}
	}
	
	private void copyLinesFromWatchedToBackup(BufferedReader bufferedReader, int numOfLinesToCopy) throws IOException {
		Objects.requireNonNull(bufferedReader);
		for(int i = 0; i < numOfLinesToCopy; ++i) {
			String newLine = null;
			newLine = bufferedReader.readLine();
			crudFile.create(newLine);
		}
	}
}