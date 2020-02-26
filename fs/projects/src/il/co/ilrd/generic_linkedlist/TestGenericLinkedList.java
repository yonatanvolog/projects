package il.co.ilrd.generic_linkedlist;

import static org.junit.Assert.*;

import java.util.ConcurrentModificationException;
import java.util.Iterator;

import org.junit.*;

public class TestGenericLinkedList {
	GenericLinkedList<String> l1 = null;
	
	@Before
	public void testConstructor() {
		l1 = new GenericLinkedList<>();
	}
	
	@Test
	public void test_isEmpty() {
		assertEquals(true, l1.isEmpty());
		l1.pushFront("data1");
		assertEquals(false, l1.isEmpty());
		l1.popFront();
		assertEquals(true, l1.isEmpty());
	}

	@Test
	public void test_size() {
		assertEquals(0, l1.size());
		l1.pushFront("data1");
		assertEquals(1, l1.size());
		l1.pushFront("data2");
		assertEquals(2, l1.size());
		l1.pushFront("data3");
		assertEquals(3, l1.size());
		l1.pushFront("data4");
		assertEquals(4, l1.size());
		
		l1.popFront();
		assertEquals(3, l1.size());
		l1.popFront();
		assertEquals(2, l1.size());
		l1.popFront();
		assertEquals(1, l1.size());
		l1.popFront();
		assertEquals(0, l1.size());
	}
	
	@Test
	public void test_find() {
		GenericLinkedList<Integer> l2 = new GenericLinkedList<>();
		Integer data1 = 1;
		Integer data2 = 2;
		Integer data3 = 3;
		Integer data4 = 4;
		Integer data5 = 5;

		assertNull(l2.find(data5));
		l2.pushFront(data1);
		l2.pushFront(data2);
		l2.pushFront(data3);
		l2.pushFront(data4);
		assertNull(l2.find(data5));
		assertSame(data1 ,l2.find(data1).next()); //next because find returns iterator
		assertSame(data2 ,l2.find(data2).next());
		assertSame(data3 ,l2.find(data3).next());
		assertSame(data4 ,l2.find(data4).next());
	}
	
	@Test
	public void test_newReverse() {
		//since it's push front, its actually
		// (head)hello -> it's -> me
		l1.pushFront("me");
		l1.pushFront("it's");
		l1.pushFront("hello");
		
		GenericLinkedList<String> reversed = GenericLinkedList.newReverse(l1);
		Iterator<String> iter = reversed.iterator();
		// checks for:
		// (head)me -> it's -> hello
		assertSame("me" ,iter.next());
		assertSame("it's" ,iter.next());
		assertSame("hello" ,iter.next());
	}
	
	@Test
	public void test_merge() {
		l1.pushFront("1");
		l1.pushFront("2");
		l1.pushFront("3");
		
		GenericLinkedList<String> l2 = new GenericLinkedList<>();
		l2.pushFront("4");
		l2.pushFront("5");
		l2.pushFront("6");
		
		GenericLinkedList<String> merged = GenericLinkedList.merge(l1, l2);
		Iterator<String> iter = merged.iterator();
		assertSame("6" ,iter.next());
		assertSame("5" ,iter.next());
		assertSame("4" ,iter.next());
		assertSame("3" ,iter.next());
		assertSame("2" ,iter.next());
		assertSame("1" ,iter.next());
	}
	
	@Test(expected = ConcurrentModificationException.class)
	public void test_failfast_push() {
		
		l1.pushFront("1");
		l1.pushFront("2");
		l1.pushFront("3");
		l1.pushFront("4");
		Iterator<String> iter = l1.iterator();
		iter.next();
		iter.next();
		iter.next();
		l1.pushFront("5");
		iter.next();
		l1.pushFront("6");
	}
	
	@Test(expected = ConcurrentModificationException.class)
	public void test_failfast_pop() {
		
		l1.pushFront("1");
		l1.pushFront("2");
		l1.pushFront("3");
		l1.pushFront("4");
		Iterator<String> iter = l1.iterator();
		iter.next();
		iter.next();
		iter.next();
		l1.popFront();
		iter.next();
		l1.pushFront("6");
	}
}
