package il.co.ilrd.factory;

import java.util.Map;
import java.util.function.Function;

import il.co.ilrd.hashMap.HashMap;

public class FactoryTest<T, K, D> {
	private Map<K, Function<D, ? extends T>> map = 
								new HashMap<K, Function<D, ? extends T>>();
	
	public void add(K key, Function<D, ? extends T> func) {
		map.put(key,func);
	}

	public T create(K key, D data) {
		return map.get(key).apply(data);
	}
	
	public T create(K key) {
		return map.get(key).apply(null);
	}
}



class Main{
	public static void main(String[] args) {
		FactoryTest<Creature, Integer, Creature> factor = new FactoryTest<>();
		
		// Lambda
		Function<Creature, Animal> animalCreate = (animal)-> new Animal();
		factor.add(1, animalCreate);
		Creature animal = factor.create(1);
		System.out.println(animal.breath());
		
		Function<Creature, Alien> alienCreate = (alien)-> new Alien();
		factor.add(2, alienCreate);
		Creature alien = factor.create(2);
		System.out.println(alien.breath());
		
		
		// Anunymos Class.
		Function<Creature, Animal> func1 = new Function<Creature, Animal>(){

			@Override
			public Animal apply(Creature arg0) {
				return new Animal();
			}
		};
		factor.add(3, func1);
		Creature animal2 = factor.create(3);
		System.out.println(animal2.makeNoise());
		
		Function<Creature, Alien> func2 = new Function<Creature, Alien>(){

			@Override
			public Alien apply(Creature arg0) {
				return new Alien();
			}
		};
		factor.add(4, func2);
		Creature alien2 = factor.create(4);
		System.out.println(alien2.makeNoise());
		
		
		// Static method.
		factor.add(5, Animal::cAnimal);
		Creature aniaml3 = factor.create(5);
		System.out.println(aniaml3.breath());
		
		// Instance method.
		Alien realAlien = new Alien();
		factor.add(6, realAlien::cAlien);
		Creature alien3 = factor.create(6);
		System.out.println(alien3.breath());
		
		
		
		// Instance Method without instnace
		factor.add(7, Creature::ctorAlien);
		Creature alien4 = factor.create(7, new Creature());
		System.out.println(alien4.breath());
		
//		StaticMethod obj = new StaticMethod();
//		factor.add(8, StaticMethod::cAlien);
//		Creature alien4 = factor.create(8);
//		System.out.println(alien4.makeNoise());
		
	}
}

class StaticMethod{
	public Alien cAlien() {
		return new Alien();
	}
	
}


class Creature{
	String breath() {
		return "Creature breath";
	}
	String makeNoise() {
		return "Creature makeNoise";
	}
	
	public Alien ctorAlien() {
		return new Alien();
	}
}
	
class Animal extends Creature{
	
	@Override
	String breath() {
		return "Animal breath";
	}
	
	@Override
	String makeNoise() {
		return "Animal makeNoise";
	}
	
	public static Animal cAnimal(Creature i) {
		return new Animal();
	}
}
	
class Alien extends Creature{
	@Override
	String breath() {
		return "Alien breath";
	}
	
	@Override
	String makeNoise() {
		return "Alien makeNoise";
	}
	
	public Alien cAlien(Creature i) {
		return new Alien();
	}
	

}