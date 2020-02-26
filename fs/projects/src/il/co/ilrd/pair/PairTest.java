package il.co.ilrd.pair;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Comparator;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class PairTest {
	
	class compareMe implements Comparator<Integer>{

		@Override
		public int compare(Integer arg0, Integer arg1) {

			return (arg0 - arg1);
		}
		
	}
	@SuppressWarnings("unchecked")
	Pair<String, Integer>[] pairarr = new Pair[10];

	@BeforeAll
	static void setUpBeforeClass() throws Exception {

	}

	@BeforeEach
	void setUp() throws Exception {
		pairarr[0] = Pair.oF("A", 10);
		pairarr[1] = Pair.oF("A", 10);
		pairarr[2] = Pair.oF("C", 30);
		pairarr[3] = Pair.oF("C", 60);
	}

	@AfterEach
	void tearDown() throws Exception {
	}

	@Test
	void testSwap() {
		Pair<Integer, String> swappedPair = Pair.swap(Pair.oF("3", 3));
		assertEquals(swappedPair.getKey(), 3);
		assertEquals(swappedPair.getValue(), "3");

	}

	@Test
	void testMinMaxTArray() {
		Integer intarr[] = {123, 1, 565, 2000, 1, 234, 1000};
		Pair<Integer, Integer> result = Pair.minMax(intarr);
		assertEquals(result.getKey(), 1);
		assertEquals(result.getValue(), 2000);
		intarr[0] = -1;
		result = Pair.minMax(intarr);
		assertEquals(result.getKey(), -1);



	}

	@Test
	void testMinMaxTArrayComparatorOfT() {
		Integer intarr[] = {123, 1, 565, 2000, 1, 234, 1000};
		Pair<Integer, Integer> result = Pair.minMax(intarr, new PairTest(). new compareMe());
		assertEquals(result.getKey(), 1);
		assertEquals(result.getValue(), 2000);
		intarr[0] = -1;
		result = Pair.minMax(intarr, new PairTest(). new compareMe());
		assertEquals(result.getKey(), -1);	}

	@Test
	void testEqualsObject() {
		assertTrue(pairarr[0].equals(Pair.oF("A", 10)));
		assertFalse(pairarr[0].equals(60));
		assertFalse(pairarr[3].equals(Pair.oF("A", "10")));

	}

}
