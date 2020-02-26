package il.co.ilrd.generic_linkedlist;

import java.util.ConcurrentModificationException;
import java.util.Iterator;

public class GenericLinkedList <E> implements Iterable<E>{
	private Node<E> head = new Node<>(null, null);
	int listModCounter = 0;

	private static class Node <E>{
		private Node<E> next;
		private E data;

		Node(E data, Node<E> next) {
			this.data = data;
			this.next = next;
		}
	}

	@SuppressWarnings("hiding") 					  // because of E shadowing
	class ListIteratorImpl<E> implements Iterator<E>{
		@SuppressWarnings("unchecked")                // because of E shadowing
		private Node<E> current = (Node<E>) head; 
		private int iterModCounter = listModCounter;

		@Override
		public boolean hasNext() {
			return (null != current.next);
		}

		@Override
		public E next() {
			if(listModCounter != iterModCounter) {
				throw new ConcurrentModificationException();
			}
			
			E nodeData = current.data;
			current = current.next;
			
			return nodeData;
		}

	} 
	public static <E> GenericLinkedList<E> merge(GenericLinkedList<E> l1,
												 GenericLinkedList<E> l2){
		GenericLinkedList<E> merged = new GenericLinkedList<E>();
		for(E data : l2) {
			merged.pushFront(data);
		}
		
		for(E data : l1) {
			merged.pushFront(data);
		}

		return newReverse(merged);
	}
	public static <E> GenericLinkedList<E> newReverse(GenericLinkedList<E> l1){
		GenericLinkedList<E> reversed = new GenericLinkedList<E>();
		for(E data : l1) {
			reversed.pushFront(data);
		}
		
		return reversed;
	}
	
	public ListIteratorImpl<E> find(E data) {	
		ListIteratorImpl<E> iter = new ListIteratorImpl<E>();
		for(E element: this) {
			if(element.equals(data)) {
				return iter;
			}
			iter.next();
		}
				
		return null;
	}
	
	public void pushFront(E data) {
		Node<E> node = new Node<E>(data, head);
		head = node;
		++listModCounter;
	}

	public boolean isEmpty() {
		return (null == head.next);
	}

	public E popFront() {
		if (this.isEmpty()) {
			return null;
		}
		E nodeData = head.data;
		Node<E> prevNode = head;
		head = head.next;
		prevNode = null;
		++listModCounter;
		
		return nodeData;
	}
	@SuppressWarnings("unused") 					 //data in list is not used
	public int size() {
		int size = 0;
		for(E e : this){
		    ++size;
		}
		
		return size;
	}
	
	@Override
	public Iterator<E> iterator() {
		return new ListIteratorImpl<E>();
	}
}