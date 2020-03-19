package il.co.ilrd.filedatabase;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Objects;

import static il.co.ilrd.filedatabase.FileBackup.*;

public class CRUDFile implements CRUD<Integer, String> {
	private FileWriter filewriter;
	private String backupFile;
	
	CRUDFile(String fileName) throws IOException {
		backupFile = fileName;
	}
	
	void setBackupFile(String backupFile) {
		this.backupFile = backupFile;
	}
	
	//Works only for adding lines
	@Override
	public Integer create(String data) {
		Objects.requireNonNull(data);
		try {
			boolean append = true;
			filewriter = new FileWriter(backupFile, append);
			filewriter.write(data + "\n");
			filewriter.close();
		} catch (IOException e) {
			//checked in compareFiles, continue	
		}
		Integer numOfLines = null;
		try {
			numOfLines = getNumOfLines(backupFile);
		} catch (IOException e) {
			// cannot re-throw
		}
		
		return numOfLines;
	}

	@Override
	public String read(Integer key) { 
		//We do not need this
		return null;
	}

	@Override
	public void update(Integer key, String newData) {
		// We don't implement this!	
	}

	@Override
	public void delete(Integer key) {
		// We don't implement this!			
	}
}