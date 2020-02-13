public class TestCircle {
	public static void main(String[] args) {
		Circle s1 = new Circle();
        System.out.println(s1.getColor());
        System.out.println("filled? " + s1.getFilled());
        System.out.println(s1.getRadius());
        System.out.println(s1.getArea());
        System.out.println(s1.getPerimeter());
        System.out.println(s1.toString());
        s1.setColor("yellow");
        s1.setFilled(false);
        s1.setRadius(3.3);
        System.out.println(s1.getColor());
        System.out.println("filled? " + s1.getFilled());
        System.out.println(s1.getRadius());

        Circle s2 = new Circle(5.5);
        System.out.println(s2.getRadius());
        System.out.println(s2.getArea());
        System.out.println(s2.getPerimeter());
        System.out.println(s2.toString());
        s2.setRadius(3.3);
        System.out.println(s2.getRadius());

        Circle s3 = new Circle ("specialcolor", false, 7.8);
        System.out.println(s3.getRadius());
        System.out.println(s3.getArea());
        System.out.println(s3.getPerimeter());
        System.out.println(s3.toString());
	}
}
