package complexnumber;
import java.util.regex.*; 

public class ComplexNumber implements Comparable<ComplexNumber> {
	private double real;
	private double imaginary;

	public ComplexNumber() {
		real = 0;
		imaginary = 0;
	}

	public ComplexNumber(double real, double imaginary) {
		this.real = real;
		this.imaginary = imaginary;
	}

	public double getReal() {
		return (real);
	}

	public void setReal(double real) {
		if (null != this) {
			this.real = real;
		}
	}

	public double getImaginary() {
		return (imaginary);
	}

	public void setImaginary(double imaginary) {
		if (null != this) {
			this.imaginary = imaginary;
		}
	}
	
	public void setValue(double real, double imaginary) {
		if (null != this) {
			this.real = real;
			this.imaginary = imaginary;
		}
	}
	
	public ComplexNumber add(ComplexNumber num) {
		if ((null != this) && (null != num)) {
			imaginary += num.imaginary;
			real += num.real;
		}
		return (this);
	}
	
	public ComplexNumber substract(ComplexNumber num) {
		if ((null != this) && (null != num)) {
			this.imaginary -= num.imaginary;
			this.real -= num.real;
		}
		return (this);
	}
	
	public ComplexNumber multiplyWith(ComplexNumber num) {
		if ((null != this) && (null != num)) {
			double temp_i = (num.imaginary * real) + (num.real * imaginary);
			double temp_r = (num.real * real) + (imaginary * num.imaginary * (-1));
			setValue(temp_r, temp_i);
		}
		return (this);
	}
	
	private ComplexNumber conjugation() {
		return (new ComplexNumber(real, imaginary * (-1)));
	}
	   
	
	public ComplexNumber divideBy(ComplexNumber num) {
		if ((null != this) && (null != num)) {
			ComplexNumber numerator = num.conjugation().multiplyWith(this);
			double denominator = (num.real * num.real) + 
								 (num.imaginary * num.imaginary);
			setValue((double)(numerator.real / denominator),
					 (double)numerator.imaginary / denominator);
				
		}
		return (this);
	}

	public Boolean isReal() {
		return (0 == imaginary);
	}
	
	public Boolean isImaginary() {
		return ((0 != imaginary) && (0 == real));
	}
	
	public static ComplexNumber parse(String complexNum) { //example: 5,3 means 5 + 3i
		if (complexNum != null) {
			Pattern pattern = Pattern.compile("( )*(-)?((\\d)+)(.)?((\\d)+)*( |,)(-)?i((\\d)+)(.)?((\\d)+)*( )*");
			Matcher matcher = pattern.matcher(complexNum);
			boolean matches = matcher.matches(); 
			
			if (true == matches)
			{
				complexNum = complexNum.trim();
				ComplexNumber result = new ComplexNumber();
				String[] array = complexNum.split("( |,)");
				result.real = Double.parseDouble(array[0]);
				result.imaginary = Double.parseDouble(array[1].replaceAll("i", ""));
				
				return (result);
			}
		}
		return (null);
	}

	@Override
	public int compareTo(ComplexNumber num) {
		if (this.equals(num)) {
			return (0);
		}
		return (1);
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (!(obj instanceof ComplexNumber)) {
			return false;
		}
		ComplexNumber object = (ComplexNumber) obj;
		
		return ((object.real == real) && (object.imaginary == imaginary));
	}

	@Override
	public int hashCode() {
		return ((int)(real * imaginary));
	}

	@Override
	public String toString() {
		
		String realString = (0 == real) ? "" : "" + real;
		String imagString = (0 == imaginary) ? "" : Math.abs(imaginary) + "i";
		String plusString = "";
		
		if ((0 == real)||(0 == imaginary)) {
			if((0 == real) && (0 > imaginary)) {
					plusString = "-";
			}else if ((0 == real) && (0 == imaginary)) {
				{
					return ("0");
				}
			}
		}
		else if (0 > imaginary) {
			plusString = "-";
		}
		else {
			plusString = "+";
		}
		
		return (realString + plusString + imagString);
	}
	
}
	
	
	