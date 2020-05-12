package il.co.ilrd.generic_linked_list;

import java.util.ConcurrentModificationException;
import java.util.Iterator;


public class GenericLinkedList <E> implements Iterable<E>{
	private Node<E> head = new Node<>(null, null);
	private int modCounter = 0;

	private class Node <N>{
		private Node<N> next;
		private N data;

		public Node(N data, Node<N> next) {
			this.data = data;
			this.next = next;
		}
	}

	public static <E> GenericLinkedList<E> merge(GenericLinkedList<E> l1, GenericLinkedList<E> l2){
		GenericLinkedList<E> merged = new GenericLinkedList<E>();
		
		for (E member : l1) {
			merged.pushFront(member);
		}
		for (E member : l2) {
			merged.pushFront(member);
		}
		merged = newReverse(merged);
		
		return merged;
	}
	
	public static <E> GenericLinkedList<E> newReverse(GenericLinkedList<E> l1){
		GenericLinkedList<E> reversed = new GenericLinkedList<E>();
		for (E mem : l1) {
			reversed.pushFront(mem);
		}
		return reversed;
		
	}
	public Iterator<E> find(E data) {
		Iterator<E> iter = new ListIteratorImpl<>();
		
		for(E member : this) {
			if (member.equals(data)) {
				return iter;
			}
			iter.next();
		}
		
		return null;
	}
	
	public void pushFront(E data) {
		++modCounter;
		Node<E> newNode = new Node<>(data, head);
		head = newNode;
	}

	public boolean isEmpty() {
		return (null == head.next);
	}

	public E popFront() {
		if(isEmpty()) {
			return null;
		}
		++modCounter;
		E data = head.data;
		head = head.next;
		
		return data;
	}

	@SuppressWarnings("unused") //iterator in "for" not needed
	public int size() {
		int size = 0;
		
		for (E memberE:this) {
			++size;
		};
		
		return size;
	}
	
	@SuppressWarnings("hiding") //Node<E> is shadowing Node<E> in LinkedList
	private class ListIteratorImpl<E> implements Iterator<E>{
		@SuppressWarnings("unchecked") //suppress type-safety - verified in implementation
		private Node<E> current = (Node<E>) head;
		private int modCheck = modCounter;

		@Override
		public boolean hasNext() {
			return (current.next != null);
		}
		
		@Override
		public E next() {
			checkMod();
			E data = current.data;
			current = current.next;
			
			return data;
		}
		
		private void checkMod() {
			if (modCheck != modCounter) {
				throw new ConcurrentModificationException();
			}
		}
	}
	
	@Override
	public Iterator<E> iterator() {
		return new ListIteratorImpl<E>();
	}
}