package il.co.ilrd.filedatabase;

public class FileWatcherTest {
	public static void main(String[] args) throws Exception {
		FileWatcher fw = new FileWatcher("/home/yonatan/myTestFile.txt");
		ChangesAnalyzer ch = new ChangesAnalyzer("/home/yonatan/myTestFile.txt","/home/yonatan/myTestFileBackup.txt");
		ch.register(fw);
	}
}