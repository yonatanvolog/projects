package il.co.ilrd.pair;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Comparator;

import org.junit.jupiter.api.*;

class PairTest<T> implements Comparator<T>{
	Pair<String, Integer> pair = null;
	
	@BeforeEach
	public void test_of() {
		pair = Pair.of("First", 1);
	}
	
	@Test
	public void test_swap() {
		Pair<Integer, String> swapped = Pair.swap(pair);
		assertTrue(pair.getKey().equals(swapped.getValue()));
		assertTrue(pair.getValue().equals(swapped.getKey()));
	}
	
	@Override									//user's compare implementation
	public int compare(T arg0,T arg1) {
		return Integer.compare((Integer)arg0, (Integer)arg1);
	}
	
	@Test
	public void test_minMax_Comparator_single() {
		Integer array[] = {5};
		Pair<Integer, Integer> res = Pair.of(null, null);
		res = Pair.minMax(array, new PairTest<>());
		
		assertEquals(5, res.getKey());
		assertEquals(5, res.getValue());
	}
	
	@Test
	public void test_minMax_Comparator_even() {
		Integer array[] = {5, 4, 9, 3, 1, 100, 9, 0};
		Pair<Integer, Integer> res = Pair.of(null, null);
		res = Pair.minMax(array, new PairTest<>());
		
		assertEquals(0, res.getKey());
		assertEquals(100, res.getValue());
	}
	
	@Test
	public void test_minMax_Comparator_noneven() {
		Integer array[] = {5, 4, 9, 3, 1, 100, 9, 0, 4};
		Pair<Integer, Integer> res = Pair.of(null, null);
		res = Pair.minMax(array, new PairTest<>());
		
		assertEquals(0, res.getKey());
		assertEquals(100, res.getValue());
	}
	
	@Test
	public void test_Comparable_minMax_single() {
		Integer array[] = {5};
		Pair<Integer, Integer> res = Pair.of(null, null);
		res = Pair.minMax(array);
		
		assertEquals(5, res.getKey());
		assertEquals(5, res.getValue());
	}
	
	@Test
	public void test_Comparable_minMax_even() {
		Integer array[] = {5, 4, 9, 3, 1, 100, 9, 0};
		Pair<Integer, Integer> res = Pair.of(null, null);
		res = Pair.minMax(array);
		
		assertEquals(0, res.getKey());
		assertEquals(100, res.getValue());
	}
	
	@Test
	public void test_Comparable_minMax_noneven() {
		Integer array[] = {5, 4, 9, 3, 1, 100, 9, 0, 4};
		Pair<Integer, Integer> res = Pair.of(null, null);
		res = Pair.minMax(array);
		
		assertEquals(0, res.getKey());
		assertEquals(100, res.getValue());
	}
	
	@Test
	public void test_getKey_getValue() {
		assertEquals("First", pair.getKey());
		assertEquals(1, pair.getValue());
	}
	
	@Test
	public void test_hashCode() {
		assertEquals(pair.hashCode(), pair.hashCode());
		
		Pair<String, Integer> pair2 = Pair.of("First", 1);
		assertEquals(pair.hashCode(), pair2.hashCode());
	}
	
	@Test //tests follow contract
	public void test_equals() {
		assertEquals(pair, pair);
		
		Pair<String, Integer> pair2 = Pair.of("First", 1);
		assertEquals(pair, pair2);

		Pair<String, Integer> pair3 = Pair.of("First", 2);
		assertNotEquals(pair3, pair);
		
		Pair<String, Integer> pair4 = Pair.of("Second", 1);
		assertNotEquals(pair4, pair);
		
		Pair<Float, String> test = Pair.of((float)2.2, "hi");
		assertNotEquals(test, pair);
		
		Pair<String, Float> test2 = Pair.of("First",(float)1);
		assertNotEquals(test2, pair);
		
		Pair<Float, Integer> test3 = Pair.of((float)5.5, 1);
		assertNotEquals(test3, pair);
	}
	
	@Test
	public void test_toString() {
		System.out.println(pair.toString());
	}
	
	@Test
	public void test_setValue() {
		pair.setValue(20);
		assertEquals(20, pair.getValue());
	}
}