public enum EWeekDay {
	SUNDAY (10) {},
	MONDAY (20) {},
	TUESDAY (30) {},
	WEDNESDAY (40) {},
	THURSDAY (50) {},
	FRIDAY (60) {},
	SATURDAY (70) {};
	
    private final int dayValue;

    private EWeekDay(int dayValue) {
        this.dayValue = dayValue;
    }
    
    public int getDayValue() {
    	return this.dayValue;
    }
    
    public void printAllValues() {
    	EWeekDay arr[] = EWeekDay.values();
    	for(EWeekDay day: arr) {
    		System.out.println(day.getDayValue());
		}
    }
    
    public EWeekDay fromValue(int value) {
    	EWeekDay arr[] = EWeekDay.values();
    	for(EWeekDay day: arr) {
    		if(day.dayValue == value) {
    			return day;
    		}    		
    	}
    	return null;

    }
}
