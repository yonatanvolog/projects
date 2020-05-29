package il.co.ilrd.raspi_clients;

import java.util.Random;

public class HeartrateSensor {
	private Random rand = new Random();
	public int getHeartRate() {
		return rand.nextInt(150);
	}
}
