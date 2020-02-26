package il.co.ilrd.vm;

public class VMProduct {
	
	private int amount;
	Product product;
	private double price;
	
	public VMProduct(int amount, Product product, double price) {
		this.amount = amount;
		this.product = product;
		this.price = price;
	}

	public int getAmount() {
		return amount;
	}
	
	public double getPrice() {
		return price;
	}

	public void setAmount(int amount) {
		this.amount = amount;
	}	
	
	public enum Product {
		COCA_COLA,
		DIET_COLA,
		SPRITE,
		FANTA,
		FUSE_TEA;
	}
}