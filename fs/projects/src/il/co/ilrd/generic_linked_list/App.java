package il.co.ilrd.generic_linked_list;

import static org.junit.Assert.assertSame;

import java.util.Iterator;

public class App {

	public static void main(String[] args) {
		GenericLinkedList<Integer> linked = new GenericLinkedList<Integer>();
		Integer a = 10;
		Integer b = 20;
		Integer c = 30;
		linked.pushFront(a);
		linked.pushFront(b);
		linked.pushFront(c);
		Iterator<Integer> iter = linked.find(30);
		assertSame(iter.next(), c);
	//	linked.popFront();
		iter.next();
		System.out.println(linked.size());
	}

}
