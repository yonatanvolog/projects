package exercise;

import java.io.PipedInputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

public class main {

	public static void main(String[] args) {
		int arr[] = {4, 1, 8 , 7};
		for(int num: arr) {
			System.out.println(num);
		}
		
		List<Integer> arrList = new ArrayList<>();
		for(int num: arr) {
			arrList.add(num);
		}
		
		arrList.sort(null);
		System.out.println(arrList.toString());
		
		Map<String, Integer> days = new HashMap<>();
		days.put("sunday", 1);
		days.put("monday", 2);
		days.put("tuesday", 3);
		days.put("wednesday", 4);
		days.put("thursday", 5);
		days.put("friday", 6);
		days.put("whooooooho", 7);

		for(Map.Entry<String, Integer> e: days.entrySet()) {
			System.out.println(e.getKey());
		}

		for(Map.Entry<String, Integer> e: days.entrySet()) {
			System.out.println(e.getValue());
		}
		
		class DataObject{
			public String code;
			public int value;
			
			public DataObject(String code, int value) {
				this.code = code;
				this.value = value;
			}
		}
		
		DataObject o1 = new DataObject("a", 1);
		DataObject o2 = new DataObject("b", 6);
		DataObject o3 = new DataObject("b", 7);
		DataObject o4 = new DataObject("a", 2);
		
		DataObject objArr[] = {o1, o2, o3, o4};
		
		Map<String, Integer> objMap = new HashMap<>();


		
		for(DataObject o:objArr) {
			if(objMap.get(o.code) != null) {
				objMap.put(o.code,o.value + objMap.get(o.code));
			}
			else {
				objMap.put(o.code,o.value);
			}
		}	

		for(Map.Entry<String, Integer> e: objMap.entrySet()) {
			System.out.println("code: " + e.getKey() + " value: " + e.getValue());
		}
	}

}
