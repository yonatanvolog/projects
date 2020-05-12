package il.co.ilrd.gatewayserver;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

public class CMDFactory<T, K, D> {
	public Map<K, Function<D, ? extends T>> map = new HashMap<>();
	
	public CMDFactory() {
		initExampleFactory();
	}
	
	public void add(K key, Function<D, ? extends T> func) {
		map.put(key, func);
	}

	public T create(K key, D data) {
		return map.get(key).apply(data);
	}
	
	public T create(K key) {
		return create(key, null);	
	}
	
	
	/**********************************************
	 * Example Factory
	 **********************************************/
	CMDFactory<FactoryCommand, CommandKey, Object> exampleFactory;

	private void initExampleFactory() {
		String data = "dummy data";
		//exampleFactory = new CMDFactory<>();

		Function<Object, FactoryCommand> companyRegCtor = (CompanyRegistration) -> new CompanyRegistration(data);
		Function<Object, FactoryCommand> productRegCtor = (ProductRegistration) -> new ProductRegistration(data);
		Function<Object, FactoryCommand> iotUserRegCtor = (IotUserRegistration) -> new IotUserRegistration(data);
		Function<Object, FactoryCommand> iotUpdateCtor = (IotUpdate) -> new IotUpdate(data);

		exampleFactory.add(CommandKey.COMPANY_REGISTRATION, companyRegCtor);
		exampleFactory.add(CommandKey.PRODUCT_REGISTRATION, productRegCtor);
		exampleFactory.add(CommandKey.IOT_USER_REGISTRATION, iotUserRegCtor);
		exampleFactory.add(CommandKey.IOT_UPDATE, iotUpdateCtor);
	}
	
	private final String PREFIX = "method executed according to - ";

	public abstract class FactoryCommand {
		String data;
		public FactoryCommand(String data) {
			this.data = data;
		}
		
		public abstract void run();
	}
	
	private class CompanyRegistration extends FactoryCommand {
		public CompanyRegistration(String data) {
			super(data);
		}
		@Override
		public void run() {
				System.out.println(PREFIX + "company registration, data is:" + data);
		}
	}
	
	private class ProductRegistration extends FactoryCommand {
		public ProductRegistration(String data) {
			super(data);
		}
		@Override
		public void run() {
				System.out.println(PREFIX + "product registration, data is:" + data);
		}
	}
	
	private class IotUserRegistration extends FactoryCommand {
		public IotUserRegistration(String data) {
			super(data);
		}
		@Override
		public void run() {
				System.out.println(PREFIX + "iot user registration, data is:" + data);
		}
	}
	
	private class IotUpdate extends FactoryCommand {
		public IotUpdate(String data) {
			super(data);
		}
		@Override
		public void run() {
				System.out.println(PREFIX + "iot update, data is:" + data);
		}
	}
}