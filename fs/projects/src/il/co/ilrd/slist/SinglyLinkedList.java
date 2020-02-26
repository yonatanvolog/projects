/**********************************************************************
 *	This code is representing my implementation of SinglyLinkedList RC2
 *	Lab: FS789                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Oved Mizrahi  		  								  *
 *	Date: 28.01.20													  *
 **********************************************************************/

package il.co.ilrd.slist;

public class SinglyLinkedList {
	private Node head = new Node(null, null);
	
	private class Node {
		private Object data;
		private Node next;

		Node(Object data ,Node next) {
			this.data = data;
			this.next = next;
		}
	}
	
	private class ListIteratorImpl implements ListIterator {
		private Node current;
		
		private ListIteratorImpl() {
			current = head;
		}
		
		@Override
		public boolean hasNext() {
			return (null != current.next);
		}
		
		@Override
		public Object next() {
			Object returnObj = current.data;
			current = current.next;
			
			return returnObj;
		}
	}
	
	public void pushFront(Object data) {
		Node pushedNode = new Node(data, head);
		head = pushedNode;
	}
	
	public Object popFront() {
		Object userData = head.data;
		head = head.next;
		
		return userData;
	}
	
	public int size() {
		int size = 0;
		
		ListIterator iter = begin();
		while(iter.hasNext()) {
			++size;
			iter.next();
		}
		
		return size;
	}
	
	public boolean isEmpty() {
		return (null == head.next);
	}
	
	public ListIterator begin() {
		ListIterator iter = new ListIteratorImpl();
		
		return iter;
	}

	public ListIterator find(Object toFind) {
		ListIterator iter = begin();
		ListIterator currentIter = begin();
		Object dataAtCurrent = null;
		
		while(iter.hasNext()) {
			dataAtCurrent = iter.next();
			if(toFind.equals(dataAtCurrent)) {
				return currentIter;
			}
			currentIter.next();

		}
		
		return null;
	}
}

