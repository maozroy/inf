package test;


public class calcMain {

	public static void main(String[] args) {
		Calculator calculator = new Calculator();
		System.out.println(calculator.operate("+", 5, 3));
		System.out.println(calculator.operate("*", 5, 3));
		System.out.println(calculator.operate("/", 15, 3));


	}

}
