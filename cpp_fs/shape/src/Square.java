public class Square extends Rectangle {
	double side;

	//no argument constructor
	Square() {
		this.side = 1.0;
	}
	
	//parameterized constructor 1
	Square(double side) {
		this.side = side;
	}
	
	//parameterized constructor 2
	Square(String color, Boolean filled, double side) {
		super(color, filled, side, side);
		this.side = side;
	}
	
	//setter
	public void setSide(double side) {
		this.side = side;
	}
	public void setWidth(double side) {
		super.setWidth(side);
	}
	public void setLength(double side) {
		super.setLength(side);
	}

	//getter
	public double getSide() {
		return this.side;
	}

	public double getArea() {
		return (this.width * this.length);
	}
	
	public double getPerimeter() {
		return (2 * (this.width + this.length));
	}
	
	//overridden toString
	public String toString() {
		return "A Square with side=" + this.side + ",which is a subclass of " + super.toString();
	}
}

