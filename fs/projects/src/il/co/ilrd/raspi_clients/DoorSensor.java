package il.co.ilrd.raspi_clients;

import java.util.Random;

public class DoorSensor {
	private Random randTimeOpen = new Random();

	public int getTimeOpen() {
		return randTimeOpen.nextInt(70);
	}
}
