package il.co.ilrd.complex_number;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class complex {

	ComplexNumber num1;
	ComplexNumber num2;
	ComplexNumber num3;
	
	@BeforeEach
	void setUp() throws Exception {
		num1 = new ComplexNumber();
		num2 = new ComplexNumber();
		num3 = new ComplexNumber();

	}

	@AfterEach
	void tearDown() throws Exception {
		num1 = null;
		num2 = null;
	}


	@Test
	void testAdd() {
		num1.setValue(2, 3);
		num2.setValue(4, 5);
		num3.setValue(6, 8);

		num1.add(num2);
		assertEquals(num1, num3);
	}

	@Test
	void testSubstract() {
		num1.setValue(2, 3);
		num2.setValue(4, 5);
		num3.setValue(-2, -2);

		num1.substract(num2);
		assertEquals(num1, num3);
		
		}

	@Test
	void testMultiplyWith() {
		num1.setValue(2, 3);
		num2.setValue(4, 5);
		num1.multiplyWith(num2);
		assertEquals(-7, num1.getReal());
		assertEquals(22, num1.getImaginary());
		
		num1.setValue(0, -9);
		num2.setValue(22, 17);
		num1.multiplyWith(num2);
		assertEquals(153, num1.getReal());
		assertEquals(-198, num1.getImaginary());
		
		}

	@Test
	void testDivideBy() {
		num1.setValue(2, 1);
		num2.setValue(3, 4);
		num1.divideBy(num2);
		assertEquals((double)2/5, num1.getReal(), 0.1);
		assertEquals((double)1/5 * -1, num1.getImaginary(), 0.1);
		num1.setValue(5,-2);
		num2.setValue(6, 7);
		num1.divideBy(num2);
		assertEquals((double)16/85, num1.getReal(), 0.1);
		assertEquals((double)47/85 * -1, num1.getImaginary(), 0.1);
	}

	@Test
	void testIsReal() {
		num1.setValue(6, 66);
		assertTrue(!(num1.isReal()));
		num1.setValue(6, 0);
		assertTrue((num1.isReal()));
		num1.setValue(0, 0);
		assertTrue((num1.isReal()));
		num1.setValue(0, 1);
		assertTrue(!(num1.isReal()));
	}

	@Test
	void testIsImaginary() {
		num1.setValue(6, 66);
		assertTrue(!(num1.isImaginary()));
		num1.setValue(0, 66);
		assertTrue((num1.isImaginary()));
		}

	@Test
	void testParse() {
		num1.setValue(6,66);
		assertEquals(num1, ComplexNumber.parse("6,i66"));
		num1.setValue(0, -1);
		assertEquals(num1, ComplexNumber.parse("0,-i1"));
		num1.setValue(-33, 0);
		assertEquals(num1, ComplexNumber.parse("-33 i0"));
	}

	@Test
	void testCompareTo() {
		num1.setValue(-33, 0);
		num2.setValue(-33, 0);
		assertEquals(num1.compareTo(num2),0);
		num1.setValue(-33, 1);
		num2.setValue(-33, 0);
		assertNotEquals((num1.compareTo(num2)),0);
		
	}

	@Test
	void testToString() {
		num1.setValue(2, 1);
		assertEquals("2.0+1.0i", num1.toString());
		num1.setValue(-2, -1);
		assertEquals("-2.0-1.0i", num1.toString());
		num1.setValue(0, -1);
		assertEquals("-1.0i", num1.toString());
		num1.setValue(-1, -1);
		assertEquals("-1.0-1.0i", num1.toString());
		num1.setValue(1, 0);
		assertEquals("1.0", num1.toString());
		num1.setValue(0, 0);
		assertEquals("0", num1.toString());
		
	}

}
