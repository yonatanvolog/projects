public class Circle extends Shape {
	double radius;
	
	//no argument constructor
	Circle(){
		this.radius = 1.0;
	}
	
	//parameterized constructor 1
	Circle(double radius){
		this.radius = radius;
	}
	
	//parameterized constructor 2
	Circle(String color, Boolean filled, double radius){
	    super(color, filled);
		this.radius = radius;
	}
	
	//setter
	public void setRadius(double radius) {
		this.radius = radius;
	}
	
	//getter
	public double getRadius() {
		return this.radius;
	}

	public double getArea() {
		return (3.14 * this.radius * this.radius);
	}

	public double getPerimeter() {
		return (2 * 3.14 * this.radius);
	}
	
	//overridden toString
	public String toString() {
		return "A Circle with radius=" + this.radius + ",which is a subclass of " + super.toString();
	}
}