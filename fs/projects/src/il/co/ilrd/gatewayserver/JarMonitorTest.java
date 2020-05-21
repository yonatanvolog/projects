package il.co.ilrd.gatewayserver;

import il.co.ilrd.observer.Callback;

public class JarMonitorTest {
	
	public JarMonitorTest() {
		System.out.println("my callback called");
	}
	
	public static void main(String[] args) throws Exception {
		String jarDirPath = "/home/student/Yonatan-Vologdin/fs/projects/bin";
		
		JarMonitor monitor = new JarMonitor(jarDirPath);
		Callback<String> callback = new Callback<String>( (JarMonitorTest) -> new JarMonitorTest(), null);
		
		monitor.register(callback);

		System.err.println("Starting backup, do your checks");
		
		Thread.sleep(30000);
	
		System.err.println("Stopping backup");
		monitor.stopUpdate();
	}
}