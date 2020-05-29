package il.co.ilrd.raspi_clients;

import java.net.Socket;

public class FridgeIOT implements IOTDevice {
	private TempSensor tempSensor;
	private DoorSensor doorSensor;
	private final String serialNumber;
	private final int portNumber;
	private final String ip;
	
	FridgeIOT(String serialNumber, int portNumber, String ip){
		this.serialNumber = serialNumber;
		this.portNumber = portNumber;
		this.ip = ip;
	}
	
	@Override
	public void sendUpdate() {
		// TODO Auto-generated method stub

	}

	@Override
	public void sendWarning() {
		// TODO Auto-generated method stub

	}

	@Override
	public void sendError() {
		// TODO Auto-generated method stub

	}


	@Override
	public void turnOnLight() {
		// TODO Auto-generated method stub

	}

}
