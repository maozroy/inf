package il.co.ilrd.slist;

public class SinglyLinkedList {
	Node head = new Node(null, null);

	public void pushFront(Object data) {
		Node newNode = new Node(head, data);
		head = newNode;
	}

	public Object popFront() {
		if (isEmpty()) {
			return null;
		}
		Node rmNode = head;
		head = head.next;
		
		return (rmNode.getData());
	}
 
	public boolean isEmpty() {
		return (null == head.next);
	}

	public int size() {
		int counter = 0;
		ListIterator iter = begin();

		while (iter.hasNext()) {
			iter.next();
			++counter;
		}

		return (counter);
	}

	public ListIterator begin() {
		return new ListIteratorimp();
	}

	public ListIterator find(Object data) {
		ListIterator iter = begin();
		ListIterator runner = begin();

		while (iter.hasNext()) {
			if(iter.next().equals(data)) {
				return runner;
			}
			runner.next();
		}

		return null;
	}

	class Node {
		private Node next;
		private Object data;

		private Node(Node next, Object data) {
			this.next = next;
			this.data = data;
		}

		private Node getNext() {
			return next;
		}

		protected Object getData() {
			return data;
		}
	}

	private class ListIteratorimp implements ListIterator {
		private Node current = head;

		@Override
		public boolean hasNext() {
			return (current.getNext() != null);
		}

		@Override
		public Object next() {
			Object data = current.getData();
			current = current.getNext();
			
			return data;
		}
	}
}
