public class TestSquare {
	public static void main(String[] args) {
		Square s1 = new Square();
		
        System.out.println(s1.getSide());
        System.out.println(s1.toString());

        s1.setSide(9.9);
        s1.setWidth(2.1);
        s1.setLength(1.2);
        System.out.println(s1.getSide());
        System.out.println(s1.toString());        
	}
}