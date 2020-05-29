package il.co.ilrd.raspi_clients;

public class HeartMonitorIOT implements IOTDevice {
	private Battery battery;
	private HeartrateSensor heartrateSensor;
	private final String serialNumber;
	private final int portNumber;
	private final String ip;
	
	HeartMonitorIOT(String serialNumber, int portNumber, String ip){
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
