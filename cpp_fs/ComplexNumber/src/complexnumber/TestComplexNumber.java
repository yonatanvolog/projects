package complexnumber;

import static complexnumber.ComplexNumber.*;
import static org.junit.Assert.*;
import org.junit.*;

public class TestComplexNumber {
	ComplexNumber cnum = null;
	ComplexNumber tmp = null;

	
	@Before
	public void ComplexNumber() {
		cnum = new ComplexNumber(2, 3);
		tmp = new ComplexNumber();
	}
	
	@Test
	public void defaultValues() {
		ComplexNumber cnum2 = new ComplexNumber();
		assertEquals(0, cnum2.getReal(), 0.1);
		assertEquals(0, cnum2.getImaginary(), 0.1);
	}
	
	@Test
	public void ComplexNumberArgArg() {
		assertEquals(2, cnum.getReal(), 0.1);
		assertEquals(3, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void setValue() {
		cnum.setValue(4, 5);
		assertEquals(4, cnum.getReal(), 0.1);
		assertEquals(5, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void add() {
		//cnum 2+3i, cnum3 4+5i
		ComplexNumber cnum3 = new ComplexNumber(4, 5);
		tmp = cnum.add(cnum3);
		assertEquals(6, tmp.getReal(), 0.1);
		assertEquals(8, tmp.getImaginary(), 0.1);
		
		assertEquals(6, cnum.getReal(), 0.1);
		assertEquals(8, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void substract() {
		//cnum 2+3i, cnum3 4+5i
		ComplexNumber cnum3 = new ComplexNumber(4, 5);
		tmp = cnum.substract(cnum3);
		assertEquals(-2, tmp.getReal(), 0.1);
		assertEquals(-2, tmp.getImaginary(), 0.1);
		
		assertEquals(-2, cnum.getReal(), 0.1);
		assertEquals(-2, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void multiplyWithPlusPlus() {
		cnum.setValue(2, 3);
		ComplexNumber cnumtest = new ComplexNumber(4, 5);
		tmp = cnum.multiplyWith(cnumtest);
		assertEquals(-7, tmp.getReal(), 0.1);
		assertEquals(22, tmp.getImaginary(), 0.1);
	
		assertEquals(-7, cnum.getReal(), 0.1);
		assertEquals(22, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void multiplyWithMinusMinus() {
		cnum.setValue(2, -3);
		ComplexNumber cnumtest = new ComplexNumber(4, -5);
		tmp = cnum.multiplyWith(cnumtest);
		assertEquals(-7, tmp.getReal(), 0.1);
		assertEquals(-22, tmp.getImaginary(), 0.1);
	
		assertEquals(-7, cnum.getReal(), 0.1);
		assertEquals(-22, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void multiplyWithPlusMinus() {
		cnum.setValue(2, 3);
		ComplexNumber cnumtest = new ComplexNumber(4, -5);
		tmp = cnum.multiplyWith(cnumtest);
		assertEquals(23, tmp.getReal(), 0.1);
		assertEquals(2, tmp.getImaginary(), 0.1);
	
		assertEquals(23, cnum.getReal(), 0.1);
		assertEquals(2, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void multiplyWithMinusPlus() {
		cnum.setValue(2, -3);
		ComplexNumber cnumtest = new ComplexNumber(4, 5);
		tmp = cnum.multiplyWith(cnumtest);
		assertEquals(23, tmp.getReal(), 0.1);
		assertEquals(-2, tmp.getImaginary(), 0.1);
	
		assertEquals(23, cnum.getReal(), 0.1);
		assertEquals(-2, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void divideByPlusPlus() {
		cnum.setValue(2, 3);
		ComplexNumber cnumtest = new ComplexNumber(4, 5);
		tmp = cnum.divideBy(cnumtest);
		assertEquals((double)23/41, tmp.getReal(), 0.1);
		assertEquals((double)2/41, tmp.getImaginary(), 0.1);
	
		assertEquals((double)23/41, cnum.getReal(), 0.1);
		assertEquals((double)2/41, cnum.getImaginary(), 0.1);
		
//		System.out.println("mine:" + cnum.getReal() + cnum.getImaginary());
//		System.out.println("expected:" + (double)23/41 + (double)2/41);
	}
	
	@Test
	public void divideByMinusMinus() {
		cnum.setValue(2, -3);
		ComplexNumber cnumtest = new ComplexNumber(4, -5);
		tmp = cnum.divideBy(cnumtest);
		assertEquals((double)23/41, tmp.getReal(), 0.1);
		assertEquals((double)-2/41, tmp.getImaginary(), 0.1);
	
		assertEquals((double)23/41, cnum.getReal(), 0.1);
		assertEquals((double)-2/41, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void divideByPlusMinus() {
		cnum.setValue(2, 3);
		ComplexNumber cnumtest = new ComplexNumber(4, -5);
		tmp = cnum.divideBy(cnumtest);
		assertEquals((double)-7/41, tmp.getReal(), 0.1);
		assertEquals((double)22/41, tmp.getImaginary(), 0.1);
	
		assertEquals((double)-7/41, cnum.getReal(), 0.1);
		assertEquals((double)22/41, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void divideByMinusPlus() {
		cnum.setValue(2, -3);
		ComplexNumber cnumtest = new ComplexNumber(4, 5);
		tmp = cnum.divideBy(cnumtest);
		assertEquals((double)-7/41, tmp.getReal(), 0.1);
		assertEquals((double)-22/41, tmp.getImaginary(), 0.1);
	
		assertEquals((double)-7/41, cnum.getReal(), 0.1);
		assertEquals((double)-22/41, cnum.getImaginary(), 0.1);
	}
	
	@Test
	public void isReal() {
		cnum.setValue(2, -3);
		assertFalse(cnum.isReal());
		
		cnum.setValue(0, -3);
		assertFalse(cnum.isReal());
		
		cnum.setValue(2, 0);
		assertTrue(cnum.isReal());
		
		cnum.setValue(0, 0);
		assertTrue(cnum.isReal());	
	}
	
	@Test
	public void isImaginary() {
		cnum.setValue(2, -3);
		assertFalse(cnum.isImaginary());
		
		cnum.setValue(0, -3);
		assertTrue(cnum.isImaginary());
		
		cnum.setValue(2, 0);
		assertFalse(cnum.isImaginary());
		
		cnum.setValue(0, 0);
		assertFalse(cnum.isImaginary());	
	}
	
	@Test
	public void parseTest() {
		
		cnum.setValue(223, -334);
		parse("  223,-334i");
		assertEquals(cnum, parse("  223,-i334"));
		
		cnum.setValue(4234, -3123);
		assertEquals(cnum, parse("  4234 -i3123"));
		
		cnum.setValue(2213112, 104343);
		assertEquals(cnum, parse(" 2213112 i104343"));
		
		cnum.setValue(123212, 343434);
		assertEquals(cnum, parse("123212,     i343434"));
		
		cnum.setValue(0,0);
		assertEquals(cnum, parse("  0    i0  "));

		//bad
		cnum.setValue(0,-3);
		assertNotEquals(cnum, parse("-3"));
		
		cnum.setValue(1,-3);
		assertNotEquals(cnum, parse(" 1,asd-i3"));
		
		cnum.setValue(2,0);
		assertNotEquals(cnum, parse("  2,,a , i0"));
		
		cnum.setValue(0,3);
		assertNotEquals(cnum, parse("0, a, b, c, i3"));
	}
	
	@Test
	public void toStringTest() {
		cnum.setValue(2, -3);
		System.out.println(cnum.toString());
		assertEquals("2.0-3.0i", cnum.toString());
		
		
		cnum.setValue(0, -3);
		System.out.println(cnum.toString());
		//assertEquals("0.0-3.0i", cnum.toString());
		
		cnum.setValue(2, 0);
		System.out.println(cnum.toString());
		//assertEquals("2.0+0.0i", cnum.toString());
		
		cnum.setValue(-2, 5);
		System.out.println(cnum.toString());
		//assertEquals("-2.0+5.0i", cnum.toString());
		
		cnum.setValue(0, 0);
		System.out.println(cnum.toString());
		//assertEquals("0.0+0.0i", cnum.toString());
	}
	
	@Test
	public void compareTo() {
		cnum.setValue(2, 3);
		ComplexNumber cnumtest = new ComplexNumber(2, 3);
		assertEquals(1 ,cnum.compareTo(cnumtest));

		cnum.setValue(3, 3);
		assertEquals(0 ,cnum.compareTo(cnumtest));
	}
	
	@Test
	public void equals() {
		cnum.setValue(2, 3);
		ComplexNumber cnumtest = new ComplexNumber(2, 3);
		assertTrue(cnum.equals(cnumtest));

		cnum.setValue(4, 3);
		assertFalse(cnum.equals(cnumtest));
		
		String dummy = new String();
		assertFalse(cnum.equals(dummy));
	}
	
	
	@After
	public void cleanup() {
		cnum = null;
		tmp = null;
	}
	
}
