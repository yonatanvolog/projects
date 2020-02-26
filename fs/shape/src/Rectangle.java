public class Rectangle extends Shape {
	double width;
	double length;

	//no argument constructor
	Rectangle() {
		this.width = 1.0;
		this.length = 1.0;
	}
	
	//parameterized constructor 1
	Rectangle(double width, double length) {
		this.width = width;
		this.length = length;
	}
	
	//parameterized constructor 2
	Rectangle(String color, Boolean filled, double width, double length) {
	    super(color, filled);
	    this.width = width;
		this.length = length;
	}
	
	//setter
	public void setWidth(double width) {
		this.width = width;
	}
	public void setLength(double length) {
		this.length = length;
	}
	
	//getter
	public double getWidth() {
		return this.width;
	}
	public double getLength() {
		return this.length;
	}

	public double getArea() {
		return (this.width * this.length);
	}
	
	public double getPerimeter() {
		return (2 * (this.width + this.length));
	}
	
	//overridden toString
	public String toString() {
		return "A Rectangle with width=" + this.width + " and length=" + this.length +",which is a subclass of " + super.toString();
	}
}
