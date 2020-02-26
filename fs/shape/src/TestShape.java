public class TestShape {
	public static void main(String[] args) {
		Shape s1 = new Shape();
		
        System.out.println(s1.color);
        System.out.println(s1.filled);
        
        System.out.println(s1.getColor());
        System.out.println(s1.getFilled());
        
        System.out.println(s1.toString());

        s1.setColor("yellow");
        s1.setFilled(false);
        System.out.println(s1.color);
        System.out.println(s1.filled);
        
        System.out.println(s1.toString());
	}
}
