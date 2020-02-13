
public class enumTest {

	public static void main(String[] args) {
		EWeekDay sun = EWeekDay.SUNDAY;
		System.out.println(sun.getDayValue());
		sun.printAllValues();
		
		System.out.println("fromValue" + sun.fromValue(70).getDayValue());
		System.out.println("fromValue" + sun.fromValue(60).getDayValue());
		System.out.println("fromValue" + sun.fromValue(50).getDayValue());
		System.out.println("fromValue" + sun.fromValue(40).getDayValue());
		System.out.println("fromValue" + sun.fromValue(30).getDayValue());
	}

}
