package slist;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;


class TestUnit {

	SinglyLinkedList myList;
	Integer Num1 = 10;
	Integer Num2 = 20;
	Integer Num3 = 30;
	Integer Num4 = 40;
	Integer Num5 = 50;
	Integer Num6 = 60;
	
	
	@BeforeEach
	public void Init() {
		myList = new SinglyLinkedList();
	}
	@AfterEach
	public void Close() {
		myList = null;
	}
	
	@Test
	void testPushFront() {

		Assert.assertNull("FIRST",myList.head.getData());
		myList.pushFront(Num1);
		Assert.assertTrue("FIRST",myList.head.getData().equals(10));
		myList.pushFront(Num2);
		Assert.assertTrue("FIRST",myList.head.getData().equals(20));

		myList.pushFront(Num3);
		myList.pushFront(Num4);
		myList.pushFront(Num5);
		Assert.assertTrue("FIRST",myList.head.getData().equals(50));

		myList.pushFront(Num6);
		Assert.assertTrue("FIRST",myList.head.getData().equals(60));

	}

	@Test
	void testPopFront() {
		Assert.assertNull("FIRST",myList.head.getData());
		Assert.assertNull(myList.popFront());
		myList.pushFront(Num2);
		Assert.assertTrue("FIRST",myList.head.getData().equals(20));
		Assert.assertEquals(myList.popFront(), Num2);
		myList.pushFront(Num3);
		Assert.assertEquals(myList.popFront(), Num3);
		Assert.assertNull(myList.popFront());
		myList.pushFront(Num4);
		myList.pushFront(Num5);
		Assert.assertEquals(myList.popFront(), Num5);
		Assert.assertEquals(myList.popFront(), Num4);
	}
	@Test
	void testIsEmpty() {
		Assert.assertEquals(myList.isEmpty(), true);
		myList.pushFront(Num1);
		Assert.assertEquals(myList.isEmpty(), false);
		myList.pushFront(Num2);
		Assert.assertEquals(myList.isEmpty(), false);
		myList.pushFront(Num3);
		myList.popFront();
		Assert.assertEquals(myList.isEmpty(), false);
		myList.popFront();
		myList.popFront();
		Assert.assertEquals(myList.isEmpty(), true);
		myList.pushFront(Num3);
	}

	@Test
	void testSize() {
		Assert.assertEquals(myList.size(), 0);
		myList.pushFront(Num1);
		Assert.assertEquals(myList.size(), 1);
		myList.pushFront(Num1);
		Assert.assertEquals(myList.size(), 2);
		myList.popFront();
		Assert.assertEquals(myList.size(), 1);
		myList.popFront();
		Assert.assertEquals(myList.size(), 0);
	}

	@Test
	void testBegin() {
		Assert.assertEquals(myList.begin().next(), null);
		myList.pushFront(Num1);
		Assert.assertEquals(myList.begin().next(), Num1);
		myList.pushFront(Num2);
		myList.pushFront(Num3);
		myList.popFront();
		myList.popFront();
		myList.popFront();
		myList.pushFront(Num3);
		Assert.assertEquals(myList.begin().next(), Num3);
	}

	@Test
	void testFind() {
		Assert.assertEquals(myList.find(Num1), null);
		myList.pushFront(Num1);
		Assert.assertEquals(myList.find(Num1).next(), Num1);
		Assert.assertEquals(myList.find(Num2), null);
		myList.pushFront(Num2);
		myList.pushFront(Num3);
		myList.pushFront(Num4);
		Assert.assertEquals(myList.find(Num3).next(), Num3);
		Assert.assertEquals(myList.find(Num1).next(), Num1);
		Assert.assertEquals(myList.find(Num4).next(), Num4);


	}

}
