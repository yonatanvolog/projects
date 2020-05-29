package il.co.ilrd.raspi_clients;

import java.util.Random;

public class WindSpeedSensor {
	private Random randWindSpeed = new Random();
	public double getWindSpeed() {
		return randWindSpeed.nextDouble() * 100;
	}
}
