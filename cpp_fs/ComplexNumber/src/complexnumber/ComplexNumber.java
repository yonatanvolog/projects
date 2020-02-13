package complexnumber;

import java.lang.Double;
import java.util.regex.*;

public class ComplexNumber implements Comparable<ComplexNumber> {
	private double real = 0.0;
	private double imaginary = 0.0;

	public ComplexNumber() {}

	public ComplexNumber(double real, double imaginary) {
		this.real = real;
		this.imaginary = imaginary;
	}

	public double getReal() {
		return real;
	}

	public void setReal(double real) {
		this.real = real;
	}

	public double getImaginary() {
		return imaginary;
	}

	public void setImaginary(double imaginary) {
		this.imaginary = imaginary;
	}
	
	public void setValue(double real, double imaginary) {
		this.real = real;
		this.imaginary = imaginary;
	}
	
	public ComplexNumber add(ComplexNumber num) {
		if(null == num) {
			return null;
		}
		
		real += num.real;
		imaginary += num.imaginary;
		
		return this;
	}
	
	public ComplexNumber substract(ComplexNumber num) {
		if(null == num) {
			return null;
		}
		
		real -= num.real;
		imaginary -= num.imaginary;
		
		return this;
	}
	
	public ComplexNumber multiplyWith(ComplexNumber num) {
		if(null == num) {
			return null;
		}
		
		double a = real;
		double b = imaginary;
		double c = num.real;
		double d = num.imaginary;
		this.real = a*c - b*d;
		this.imaginary = a*d + b*c;
		
		return this;
	}
	
	public ComplexNumber divideBy(ComplexNumber num) {
		if(null == num) {
			return null;
		}
		
		double a = real;
		double b = imaginary;
		double c = num.real;
		double d = num.imaginary;
		real = (a*c+b*d)/(c*c+d*d);
		imaginary = (b*c-a*d)/(c*c+d*d);
		
		return this;
	}

	public Boolean isReal() {
		return (0 == imaginary);
	}
	
	public Boolean isImaginary() {
		return ((0 != imaginary) && (0 == real));
	}
	
	public static ComplexNumber parse(String complexNum) { //example: 5, -i3 means 5 - 3i
		String regex = "(?:[ ]*)([-]?\\d+)(?:[ ]*)[ ,](?:[ ]*)([-]?)(?:i)(\\d+)(?:[ ]*)$";
		int imaginarySign = 1;
		Pattern pattern = Pattern.compile(regex);
		Matcher matcher = pattern.matcher(complexNum);
		ComplexNumber cnum = new ComplexNumber();
		
		while(matcher.find()) {
			cnum.real = Double.parseDouble(matcher.group(1));
			if(matcher.group(2).equals("-")) {
				imaginarySign *= -1;
			}
			cnum.imaginary = Double.parseDouble(matcher.group(3)) * imaginarySign;
		}

		return cnum;
	}

	@Override
	public int compareTo(ComplexNumber num) {
		if(null == num) {
			return 1;
		}
		
		return (((num.real == real) && (num.imaginary == imaginary)) ? 1 : 0);
	}

	@Override
	public boolean equals(Object num) {
		if((null != num) && (num instanceof ComplexNumber)) {
			ComplexNumber cnum = (ComplexNumber)num;
			
			return ((cnum.real == real) && (cnum.imaginary == imaginary));
		}
		
		return false;
	}

	@Override
	public int hashCode() {
		return (int)(real * imaginary);
	}

	@Override
	public String toString() {
        String imaginarySign = "";
        if(0 <= imaginary) {
        	imaginarySign = "+";
        }
		
		return (real + "" + imaginarySign + imaginary + "i");
	}
}