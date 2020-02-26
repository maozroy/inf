package il.co.ilrd.hashMap;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class HashTest {

	@BeforeAll
	static void setUpBeforeClass() throws Exception {
	}

	@AfterAll
	static void tearDownAfterClass() throws Exception {
	}

	@BeforeEach
	void setUp() throws Exception {
	}

	@AfterEach
	void tearDown() throws Exception {
	}

	@Test
	void get() {
		HashMap<Integer, String> myhash = new HashMap<Integer, String>(2);
		myhash.put(1, "first");
		assertNull(myhash.get(4));
		
	}
	
	@Test
	void remove() {
		HashMap<Integer, String> myhash = new HashMap<Integer, String>(2);
		myhash.put(1, "first");
		myhash.put(2, "sec");
		myhash.put(3, "thir");
		myhash.put(4, "fourt");
		myhash.remove(3);
		assertEquals(3, myhash.size());
		assertFalse(myhash.containsValue("thir"));
		

	}
	
	@Test
	void put() {
		HashMap<Integer, String> myhash = new HashMap<Integer, String>(2);
		assertNull(myhash.put(1, "first"));
		assertEquals(1, myhash.size());
		myhash.put(2, "first");
		assertEquals(2, myhash.size());
		myhash.put(2, "second");
		assertEquals("second", myhash.get(2));
		assertEquals(2, myhash.size());
		myhash.put(2, "first");
		assertEquals(2, myhash.size());
	}
	
	@Test
	void ctor() {
		HashMap<Integer, String> myhash = new HashMap<Integer, String>();
		assertEquals(myhash.size(), 0);

	}
	
	@Test
	void testisempty() {
		HashMap<Integer, String> hashMap1 = new HashMap<Integer, String>(6);
		assertTrue(hashMap1.isEmpty());
		hashMap1.put(1, "value 3");
		hashMap1.put(2, "value 3");
		hashMap1.put(3, "value 3");
		hashMap1.put(4, "value 3");
		assertFalse(hashMap1.isEmpty());
	}
	
	@Test
	void testcontainvalue() {
		HashMap<Integer, String> hashMap1 = new HashMap<Integer, String>(6);
		hashMap1.put(1, "value 2");
		hashMap1.put(2, "val");
		hashMap1.put(3, "value 3");
		hashMap1.put(4, "value 3");
		assertFalse(hashMap1.containsValue("value"));
		assertTrue(hashMap1.containsValue("val"));
	}
	
	
	@Test
	void testclear() {
		HashMap<Integer, String> hashMap1 = new HashMap<Integer, String>(6);
		hashMap1.put(1, "value 3");
		hashMap1.put(2, "value 3");
		hashMap1.put(3, "value 3");
		hashMap1.put(4, "value 3");
		hashMap1.clear();
		assertEquals(0, hashMap1.size());
		hashMap1.put(1, "value 3");
		hashMap1.put(2, "value 3");
		hashMap1.put(3, "value 3");
		hashMap1.put(4, "value 3");
		assertEquals(4, hashMap1.size());
	}
	
	@Test
	void testputall() {
		HashMap<Integer, String> hashMap1 = new HashMap<Integer, String>(6);
		HashMap<Integer, String> hashMap2 = new HashMap<Integer, String>(6);
		hashMap1.put(1, "value 3");
		hashMap1.put(2, "value 3");
		hashMap1.put(3, "value 3");
		hashMap1.put(4, "value 3");
		hashMap2.put(1, "value 3");
		hashMap2.put(5, "value 3");
		hashMap2.put(3, "value 3");
		hashMap2.put(4, "value 3");
		hashMap1.putAll(hashMap2);
		assertEquals(5, hashMap1.size());

		
		
	}
	

	@Test
	void IteratorTest() {
	HashMap<Integer, String> hashMap1 = new HashMap<Integer, String>(6);
	HashMap<Integer, String> hashMap2 = new HashMap<Integer, String>(6);
//	hashMap1.put(0, "value 2");
//	hashMap1.put(1, "value 3");
//	hashMap1.put(2, "value 2");
//	hashMap1.put(3, "value 3");
//	hashMap1.put(4, "value 3");
	hashMap1.put(5, "value 3");
	hashMap2.put(13, "value 3");
	hashMap2.put(14, "value 3");
	hashMap2.put(15, "value 3");
	hashMap1.putAll(hashMap2);
	for(String value : hashMap2.values()) {
		System.out.println(value);
	}
	
	}
}
