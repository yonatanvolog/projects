package il.co.ilrd.vm;

public class TestVendingMachine {
	public static void main(String[] args) {		
		VMachine vm1 = new VMachine();
		vm1.start();
		
		vm1.selectItem(VMProduct.Product.COCA_COLA);
		vm1.addMoney(1);
		System.out.println("check: 1.0 == " + vm1.getCurrentMoney() + "?");
		
		vm1.addMoney(1.5);
		System.out.println("check: 2.5 == " + vm1.getCurrentMoney() + "?");
		
		vm1.selectItem(VMProduct.Product.COCA_COLA);
		System.out.println("check: 0.0 == " + vm1.getCurrentMoney() + "?");
		
		vm1.addMoney(5.0);
		System.out.println("check: 5.0 == " + vm1.getCurrentMoney() + "?");
		
		vm1.selectItem(VMProduct.Product.COCA_COLA);
		System.out.println("check: 0.0 == " + vm1.getCurrentMoney() + "?");
		
		//testing timeout
		System.out.println("***Tesing timout***");
		vm1.addMoney(10.0);
		System.out.println("check: 10.0 == " + vm1.getCurrentMoney() + "?");
		
		//sleep 10 seconds
		try {
			Thread.sleep(10 * 1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		vm1.selectItem(VMProduct.Product.COCA_COLA);
		vm1.addMoney(6.5);
		System.out.println("check: 6.5 == " + vm1.getCurrentMoney() + "?");
		
		vm1.getRefund();
		System.out.println("check: 0.0 == " + vm1.getCurrentMoney() + "?");
		
		vm1.turnOff();
	}
}