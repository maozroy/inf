package il.co.ilrd.pair;

import java.util.Comparator;
import java.util.Map;

public class Pair<T, U> implements Map.Entry<T, U> {
	
	private T first;
	private U second;
	
	private Pair(T first, U second) {
		this.first = first;
		this.second = second;
	};
	
	public static<T,U> Pair<T, U> oF(T first, U second){
		Pair<T, U> newPair = new Pair<>(first, second);
		
		return newPair;
	}
	
	public static<T,U> Pair<U, T> swap(Pair<T, U> pair){
		
		return oF(pair.second, pair.first);
	}
	
	private static<T> Pair<T, T> baseMinMax(T[] array, MinMaxComp<T> lambda){

		if(null == array || 0 == array.length) {
			return null;
		}
	
		if(1 == array.length) {
			return oF(array[0], array[0]);
		}
		
		T minT = array[0];
		T maxT = array[0];
		T tempMinT;
		T tempMaxT;
		
		for(int i = 0 ; i < (array.length/2 + 1) ; i++) {
			
			
			if(lambda.comparing(array[array.length - i - 1], array[i]) <= 0) {
				tempMaxT = array[i];
				tempMinT = array[array.length - i - 1];
			}
			else{
				tempMaxT = array[array.length - i - 1];
				tempMinT = array[i];
			}
			
			if(lambda.comparing(tempMinT, minT) <= 0) {
				minT = tempMinT;
			}
			if(lambda.comparing(tempMaxT, maxT) >= 0) {
				maxT = tempMaxT;
			}
		}
		
		return oF(minT, maxT);
	}

	public static<T extends Comparable<T>> Pair<T, T> minMax(T[] array){
		
		MinMaxComp<T> lambda = (T t1, T t2) -> {
			return ((Comparable<T>) t1).compareTo(t2);
		};
		
		return baseMinMax(array, lambda);
	}

	public static<T> Pair<T, T> minMax(T[] array, Comparator<T> comp){
		
		MinMaxComp<T> lambda = (T t1, T t2) -> {
			return comp.compare(t1, t2);
		};
		
		return baseMinMax(array, lambda);
	}

/******************************************************************************/
	@Override
	public T getKey() {
		return first;
	};
	@Override
	public U getValue() {
		return second;
	};
	@Override
	public int hashCode() {
		return first.hashCode();
	}
	
	@Override
	public boolean equals(Object o) {
		if(!(o instanceof Pair)) {
			return false;
		}
		
		Pair<T, U> o_pair= (Pair<T, U>) o;
		if(!(o_pair.first.getClass().equals(first.getClass())) || 
		   !(o_pair.second.getClass().equals(second.getClass()))) {
			return false;
		}
		
		return ((o_pair.first.equals(first)) && 
				(o_pair.second.equals(second)));
	}
	
	@Override
	public String toString() {
		return first.toString() + " " + second.toString();
	}
	@Override
	public U setValue(U value) {
		U toReturn = second;
		second = value;
		
		return toReturn;
	}
}