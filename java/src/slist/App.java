package slist;

public class App {
	public static void main(String[] args) {
		SinglyLinkedList myList = new SinglyLinkedList();
		Integer Num1 = 10;
		Integer Num2 = 20;
		Integer Num3 = 30;
		Integer Num4 = 40;
		Integer Num5 = 50;
		Integer Num6 = 60;
		System.out.println("list should be empty! is it? " + myList.isEmpty());
		myList.pushFront(Num1);
		myList.pushFront(Num2);
		myList.pushFront(Num3);
		myList.pushFront(Num4);
		myList.pushFront(Num5);
		myList.pushFront(Num6);
		System.out.println("size should be 6 and its " + myList.size());
		myList.popFront();
//		myList.popFront();
	//	myList.popFront();
//		myList.popFront();
		ListIterator iter = myList.find(30);
	//	iter.next();
		ListIterator iter2 = myList.begin();
		System.out.println("begin is " + iter2.next());

		System.out.println("search 30 iter is " + iter.next());
		iter = myList.find(60);
		System.out.println("search 60 iter should be null " + iter);
		Integer Popped = (Integer)myList.popFront();
		System.out.println(Popped);

		System.out.println(myList.isEmpty());
		
	}
}
