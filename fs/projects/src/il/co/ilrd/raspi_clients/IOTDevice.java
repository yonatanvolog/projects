package il.co.ilrd.raspi_clients;

public interface IOTDevice {
	public void sendUpdate();
	public void sendWarning();
	public void sendError();
	public void turnOnLight();
}
