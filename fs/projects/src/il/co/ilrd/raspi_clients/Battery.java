package il.co.ilrd.raspi_clients;

public class Battery extends Thread {
	private double batteryPower;
	private final double dangerZone = 20;

	Battery(double initialValue){
		this.batteryPower = initialValue;
	}
	
	public boolean isLow() {
		return --batteryPower < dangerZone;
	}

	//battery drainage simulation
	@Override
	public void run() {
		while(true) {
			try {
				Thread.sleep(3000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			if(batteryPower > 0) {
				--batteryPower;
			}
		}
	}
}
