package test;

import java.util.List;

interface Person{
	default void sayHello() {
		System.out.println("Hello");
	}
	void Fuck();
}
interface Male extends Person{
	default void sayHello() {
		System.out.println("Hi");
	}
}

class Sam <T>implements Person{

	@Override
	public void Fuck() {
		// TODO Auto-generated method stub
		
	}
}




