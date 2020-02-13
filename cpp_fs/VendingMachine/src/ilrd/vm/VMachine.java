package ilrd.vm;
import ilrd.vm.VMProduct.Product;

public class VMachine {

	private double currentMoney = 0;
	private States currentState = States.INIT;
	VMProduct[] productsInMachine;
	private Monitor monitor = new TestMonitor();
	TimeoutThread timerThread;
	
	{
		productsInMachine = initArray();
	}

	public void start()	{
		currentState.start(this);
	}
	
	public void addMoney(double money) {
		currentState.addMoney(this, money);
	}
	
	public void selectItem(Product selectedProduct) {		
		currentState.selectItem(this, selectedProduct);
	}
	
	public void getRefund() {
		currentState.getRefund(this);		
	}
	
	public void turnOff() {
		currentState.turnOff(this);
	}

	public double getCurrentMoney() {
		return currentMoney;
	}

	public void setCurrentMoney(double currentMoney) {
		this.currentMoney = currentMoney;
	}
	
	void notificationFromTimer() {
		currentState.timeoutHandler(this);
	}
	
	public enum States {
		INIT {
			@Override
			public void start(VMachine vm) {
				vm.timerThread = new TimeoutThread(vm);
				vm.timerThread.startTimer();
				vm.monitor.print("Starting Machine");
				vm.currentState = WAIT_FOR_MONEY;
			}
		},
		WAIT_FOR_MONEY {
			@Override
			public void selectItem(VMachine vm, Product selectedProduct) {}

			@Override
			public void turnOff(VMachine vm) {
				vm.timerThread.stopTimer();
				vm.currentState = INIT;
				vm.monitor.print("Turning Machine OFF");
			}	
		},
		WAIT_FOR_CHOOSING {
			@Override
			public void selectItem(VMachine vm, Product selectedProduct) {
				/*check if enough products available*/
				if(vm.hasEnoughMoneyImpl(selectedProduct)) {
				   vm.monitor.print("Vending product out a " + selectedProduct);
				   vm.setCurrentMoney(vm.getCurrentMoney() - 
				   vm.productsInMachine[selectedProduct.ordinal()].getPrice());
				   /*decrease amount of this product*/
				}
				else {
					vm.monitor.print("Not enough money");
				}
				getRefund(vm);
			}

			@Override
			public void getRefund(VMachine vm) {
				if(vm.getCurrentMoney() >= 0) {
					vm.monitor.print("Refunded " + vm.getCurrentMoney());
					vm.setCurrentMoney(0);
					vm.currentState = WAIT_FOR_MONEY;
				}
			}
			
			@Override
			public void timeoutHandler(VMachine vm) {
				this.getRefund(vm);
			};
		};
		public void addMoney(VMachine vm, double money) {
			TimeoutThread.timeoutCounter = 0;
			vm.setCurrentMoney(vm.getCurrentMoney() + money);
			vm.currentState = WAIT_FOR_CHOOSING;
			
			vm.monitor.print("Inserted " + money + ", now inside: " 
			        								+ vm.getCurrentMoney());
			}
		public void selectItem(VMachine vm, Product selectedProduct) {};
		public void getRefund(VMachine vm) {};
		public void start(VMachine vm) {};
		public void turnOff(VMachine vm) {};
		public void timeoutHandler(VMachine vm) {};
	}
	
	private VMProduct[] initArray() {
		return new VMProduct[] {
				new VMProduct(10, VMProduct.Product.COCA_COLA, 4.5),
				new VMProduct(10, VMProduct.Product.DIET_COLA, 5.5),
				new VMProduct(5, VMProduct.Product.SPRITE, 3.0),
				new VMProduct(8, VMProduct.Product.FANTA, 3.9),
				new VMProduct(15, VMProduct.Product.FUSE_TEA, 4.0)};
	}

	private boolean hasEnoughMoneyImpl(Product selectedProduct) {
		return this.getCurrentMoney() >=
			   this.productsInMachine[selectedProduct.ordinal()].getPrice();
	}
}