/**********************************************************************
 *	This code is representing my tests of SinglyLinkedList            *
 *	Lab: FS789                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 28.01.20													  *
 **********************************************************************/

package il.co.ilrd.slist;

import static org.junit.Assert.*;

import java.util.ConcurrentModificationException;

import org.junit.*;

public class SinglyLinkedListTest {
	SinglyLinkedList l1 = null;
	
	@Before
	public void testConstructor() {
		l1 = new SinglyLinkedList();
		assertEquals(true, l1.isEmpty());
	}
	
	@Test
	public void testConstructor1() {}
	
	@Test
	public void testpushFront() {
		Integer o1 = 1;
	
		l1.pushFront(o1);
	}
	
	@Test
	public void testpopFront() {
		Integer o1 = 1;
	
		l1.pushFront(o1);
		assertEquals(o1, l1.popFront());
	}
	
	@Test
	public void testisEmptypushFront() {
		Integer o1 = 1;
		Integer o2 = 2;
		Integer o3 = 3;
		Integer o4 = 4;
		Integer o5 = 5;
		
		assertEquals(true, l1.isEmpty());
		l1.pushFront(o1);
		assertEquals(false, l1.isEmpty());
		l1.pushFront(o2);
		l1.pushFront(o3);
		l1.pushFront(o4);
		l1.pushFront(o5);
		assertEquals(false, l1.isEmpty());
	}
	
	@Test
	public void testisEmptypopFront() {
		Integer o1 = 1;
		Integer o2 = 2;
		Integer o3 = 3;
		Integer o4 = 4;
		Integer o5 = 5;
		
		l1.pushFront(o1);
		l1.pushFront(o2);
		l1.pushFront(o3);
		l1.pushFront(o4);
		l1.pushFront(o5);
		
		assertEquals(false, l1.isEmpty());
		assertEquals(o5, l1.popFront());
		assertEquals(false, l1.isEmpty());
		assertEquals(o4, l1.popFront());
		assertEquals(o3, l1.popFront());
		assertEquals(o2, l1.popFront());
		assertEquals(o1, l1.popFront());
		assertEquals(true, l1.isEmpty());
	}
	
	@Test
	public void testSizepopFront() {
		Integer o1 = 1;
		Integer o2 = 2;
		Integer o3 = 3;
		Integer o4 = 4;
		Integer o5 = 5;
		
		l1.pushFront(o1);
		l1.pushFront(o2);
		l1.pushFront(o3);
		l1.pushFront(o4);
		l1.pushFront(o5);
		
		assertEquals(5, l1.size());
		assertEquals(o5, l1.popFront());
		assertEquals(4, l1.size());
		assertEquals(o4, l1.popFront());
		assertEquals(3, l1.size());
		assertEquals(o3, l1.popFront());
		assertEquals(2, l1.size());
		assertEquals(o2, l1.popFront());
		assertEquals(1, l1.size());
		assertEquals(o1, l1.popFront());
		assertEquals(0, l1.size());
	}

	@Test(expected = ConcurrentModificationException.class)
	public void testsize() {
		Integer o1 = 1;
		Integer o2 = 2;
		Integer o3 = 3;
		Integer o4 = 4;
		Integer o5 = 5;
		
		assertEquals(0, l1.size());
		l1.pushFront(o1);
		assertEquals(1, l1.size());
		l1.pushFront(o2);
		assertEquals(2, l1.size());
		l1.pushFront(o3);
		assertEquals(3, l1.size());
		l1.pushFront(o4);
		assertEquals(4, l1.size());
		l1.pushFront(o5);
		assertEquals(5, l1.size());
	}
	
	@Test
	public void testfind() {
		Integer o1 = 1;
		Integer o2 = 2;
		Integer o3 = 3;
		Integer o4 = 4;
		Integer o5 = 5;
		
		l1.pushFront(o1);
		l1.pushFront(o2);
		l1.pushFront(o3);
		l1.pushFront(o4);
		l1.pushFront(o5);
		
		assertEquals(o5, (l1.find(o5)).next());
		assertEquals(o4, (l1.find(o4)).next());
		assertEquals(o3, (l1.find(o3)).next());
		assertEquals(o2, (l1.find(o2)).next());
		assertEquals(o1, (l1.find(o1)).next());

		assertNotEquals(o1, (l1.find(o2)).next());
	}
	
	@After
	public void cleanup() {
		l1 = null;
	}
}
