package il.co.ilrd.filedatabase;

public class FileBackupTest {
	public static void main(String[] args) throws Exception {
		String myWatchedFile = "/home/yonatan/myTestFile.txt";
		String myBackupFile = "/home/yonatan/myTestFileBackup.txt";
		
		FileBackup fb = new FileBackup(myWatchedFile);
		fb.setBackupFile(myBackupFile);
		
		System.err.println("Starting backup, do your checks");
		fb.startFileBackup();
		
		Thread.sleep(30000);
	
		System.err.println("Stopping backup");
		fb.stopFileBackup();
	}
}