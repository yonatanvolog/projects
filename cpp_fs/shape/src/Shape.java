abstract public class Shape {
	//instance variables
	String color;
	Boolean filled;
	
	//no argument constructor
	Shape(){
		this.color = "green";
		this.filled = true;
	}
	
	//parameterized constructor
	Shape(String color, Boolean filled){
		this.color = color;
		this.filled = filled;
	}
	
	//getter
	public String getColor() {
		return color;
	}
	public Boolean getFilled() {
		return filled;
	}
	
	//setter
	public void setColor(String color) {
		this.color = color;
	}
	public void setFilled(Boolean filled_param) {
		filled = filled_param;
	}
	
	//toString
	public String toString() {
		if (true == this.filled) {
			return "A Shape with color of : " + this.color + " and filled";
		}
		return "A Shape with color of : " + this.color + " and not filled";
	}
	
	abstract public double getArea();

	abstract public double getPerimeter();
}