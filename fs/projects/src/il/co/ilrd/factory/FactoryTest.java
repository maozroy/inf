package il.co.ilrd.factory;

import static org.junit.jupiter.api.Assertions.*;

import java.util.function.Function;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;



class FactoryTest {
	
	private static class fac {
		public Animal animalcreate(Object arg0) {
			return new FactoryTest().new Animal();
		}
		static public Creature animalcreatestatic(Object arg0) {
			return new FactoryTest().new Animal();
		}
	}
	class Creature{
		
		public Creature creating() {
			return new Creature();
		}
		
		String breath() {
			return "air in air out";
		}
		String makeNoise() {
			return "basic creature sound";
		}
	}
	
	class Animal extends Creature{

		@Override
		String breath() {
			return "earth air in earth air out";
		}

		@Override
		String makeNoise() {
			return "the sound of life";
		}
	}
	
	class Alien extends Creature{
		@Override
		String breath() {
			return "space air in space air out";
		}

		@Override
		String makeNoise() {
			return "the sound of weird life";
		}
	}
	
	
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
	void testCreateKD() {
//		Factory<Creature, Integer, Object> creatureFactory = new Factory<>();
//		Function<Object, Animal> createAnimal = (animal) -> new Animal();
//		creatureFactory.add(0, createAnimal);
//		assertEquals("earth air in earth air out", creatureFactory.create(0).breath());
//		creatureFactory.add(1, new Function<Object, Animal>(){
//
//			@Override
//			public Animal apply(Object arg0) {
//				return new Animal();
//			}
//			
//			}
//		);
//		assertEquals("earth air in earth air out", creatureFactory.create(1).breath());
//		creatureFactory.add(11,new fac()::animalcreate);
//		System.out.println(creatureFactory.create(11).breath());
//		creatureFactory.add(12,fac::animalcreatestatic);
//		System.out.println(creatureFactory.create(12).breath());
//		creatureFactory.create(666, new Animal()).breath();
		Factory<Creature, Integer, Creature> newcreatureFactory = new Factory<>();
		newcreatureFactory.add(1, Creature::creating);
		System.out.println(newcreatureFactory.create(1, new Creature()).breath());

		
	}
	
	@Test
	void testCreateK() {
		fail("Not yet implemented");
	}

}
