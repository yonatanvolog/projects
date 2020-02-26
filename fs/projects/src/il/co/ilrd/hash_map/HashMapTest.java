package il.co.ilrd.hash_map;

import static org.junit.jupiter.api.Assertions.*;
import java.util.*;
import org.junit.jupiter.api.*;

class HashMapTest {

	HashMap<String, Integer> map = null;
		
	@BeforeEach
	public void HashMapCtorNoArg() {
		map = new HashMap<>();
		assertTrue(map != null);
	}
	
	@Test
	public void clear() {
		assertTrue(map.isEmpty());
		map.put("key1", 1);
		map.put("key1", 2);
		map.put("key2", 5);
		map.put("key2", 6);
		map.put("key3", 7);
		map.put("key3", 8);
		map.put("key4", 9);
		map.put("key4", 10);
		assertFalse(map.isEmpty());
		map.clear();
		assertTrue(map.isEmpty());
	}
	
	@Test
	public void containsKey() {
		assertFalse(map.containsKey("key1"));
		map.put("key1", 1);
		assertTrue(map.containsKey("key1"));
		
		map.remove("key1");
		assertFalse(map.containsKey("key1"));
		
		assertFalse(map.containsKey("key2"));
		assertFalse(map.containsKey("key3"));

		map.put("key2", 1);
		assertTrue(map.containsKey("key2"));
		map.put("key2", 2);
		assertTrue(map.containsKey("key2"));

		map.put("key3", 3);
		assertTrue(map.containsKey("key3"));		
	}
	
	@Test
	public void containsValue() {
		assertFalse(map.containsValue(1));
		map.put("key1", 1);
		assertTrue(map.containsValue(1));
		
		map.remove("key1");
		assertFalse(map.containsValue(1));
		
		map.put("key1", 2);
		assertFalse(map.containsValue(1));
		assertTrue(map.containsValue(2));
		
		map.remove("key1");
		assertFalse(map.containsValue(2));
		map.put("key2", 2);
		assertTrue(map.containsValue(2));
	}
	
	@Test
	void entrySet() {
		assertTrue(map.isEmpty());

		map.put("key1", 110);
		map.put("key1", 120);
		map.put("key2", 130);
		map.put("key2", 140);
		map.put("key3", 150);
		map.put("key4", 160);
		map.put("key5", 170);

//		this should be the result:		
//		map.put("key1", 120);
//		map.put("key2", 140);
//		map.put("key3", 150);
//		map.put("key4", 160);
//		map.put("key5", 170);
		
		assertEquals(5, map.size());
		int counter = 1;
		System.out.println("entrySet() test:");
		for(Map.Entry<String, Integer> entry: map.entrySet()) {
			System.out.println(counter + "/5 " + entry);
			++counter;
		}
	}
	
	@Test
	public void get() {		
		map.put("key1", 1);
		assertEquals(1, map.get("key1"));
		assertEquals(null, map.get("key2"));
		
		map.put("key1", 2);
		assertEquals(2, map.get("key1"));
	}
	
	@Test
	public void isEmpty() {
		assertTrue(map.isEmpty());
		map.put("key1", 1);
		assertFalse(map.isEmpty());
		map.remove("key1");
		assertTrue(map.isEmpty());
		map.put("key1", 1);
		map.put("key1", 2);
		map.put("key2", 1);
		map.put("key3", 1);
		map.remove("key1");
		map.remove("key2");
		map.remove("key3");
		assertTrue(map.isEmpty());
	}
	
	@Test
	public void keySet() {		
		map.put("key1", 110);
		map.put("key2", 120);
		map.put("key3", 130);
		map.put("key4", 140);
		map.put("key5", 150);
		map.put("key6", 160);
		map.put("key7", 170);

		Iterator<String> keyIter = map.keySet().iterator();
		
		System.out.println("keySet() test:");
		int counter = 1;
		while(keyIter.hasNext()) {
			System.out.println(counter + "/7 " + keyIter.next());
			++counter;
		}
	}
	
	@Test
	public void put() {		
		assertEquals(null, map.put("key1", 1));
		assertEquals(1, map.put("key1", 2));
		assertEquals(null, map.put("key2", 1));
		assertEquals(null, map.put("key3", 1));		
	}
	
	@Test
	public void putAll() {
		HashMap<String, Integer> map2 = new HashMap<>();
		map.put("key1", 1);
		map.put("key1", 2);
		map.put("key2", 5);
		map.put("key2", 6);
		
		map2.put("key1", 11);
		map2.put("key1", 12);
		map2.put("key2", 15);
		map2.put("key2", 16);
		map2.put("key20", 20);

		assertFalse(map.containsValue(11));		
		assertFalse(map.containsValue(12));		
		assertFalse(map.containsValue(15));	
		assertFalse(map.containsValue(16));
		assertFalse(map.containsKey("key20"));
		
		map.putAll(map2);
		assertTrue(map.containsValue(12));		
		assertTrue(map.containsValue(16));
		assertTrue(map.containsKey("key20"));
		assertEquals(3, map.size());
	}
	
	@Test
	public void remove() {
		map.put("key1", 1);
		assertEquals(1, map.remove("key1"));
		assertEquals(null, map.remove("key2"));
		
		map.put("key1", 2);
		assertEquals(2, map.remove("key1"));

		assertEquals(null, map.remove("key5"));
	}
	
	@Test
	public void size() {
		assertEquals(0 ,map.size());
		map.put("key1", 1);
		assertEquals(1 ,map.size());
		map.remove("key1");
		assertEquals(0 ,map.size());
		map.put("key1", 1);
		assertEquals(1 ,map.size());
		map.put("key1", 2);
		assertEquals(1 ,map.size());
		map.put("key2", 1);
		map.put("key3", 1);
		assertEquals(3 ,map.size());
		map.remove("key1");
		map.remove("key2");
		map.remove("key3");
		assertEquals(0 ,map.size());
		
		map.put("key1", 1);
		map.put("key2", 1);
		map.put("key3", 1);
		map.put("key4", 1);
		map.put("key5", 1);
		map.put("key6", 1);
		assertEquals(6 ,map.size());
	}
	
	@Test
	public void values() {
		map.put("key1", 110);
		map.put("key2", 120);
		map.put("key3", 130);
		map.put("key4", 140);
		map.put("key5", 150);
		map.put("key6", 160);
		map.put("key7", 170);
		
		System.out.println("values() test:");
		Iterator<Integer> valueIter = map.values().iterator();
		int counter = 1;
		while(valueIter.hasNext()) {
			System.out.println(counter + "/7 " + valueIter.next());
			++counter;
		}

//		Collection<Integer> collection = map.values();
//		for (Integer a : collection) {
//			System.out.println("enhanced for loop test");
//
//		}
	}
}
