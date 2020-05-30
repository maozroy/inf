package il.co.ilrd.pair;

import java.util.Comparator;

public class App{
	public static void main(String[] args) {

		Pair<String, Integer>[] pairarr = new Pair[10];
		Integer aInteger = 4;
		pairarr[0] = Pair.oF("A", 10);
		pairarr[1] = Pair.oF("A", 10);
		pairarr[2] = Pair.oF("C", 30);
		pairarr[2] = Pair.oF("C", 60);
		Integer intarr[] = {123, 1, 565, 2000, 1, 234, 1000};
		Pair<Integer, Integer> result = Pair.minMax(intarr,new App().new compareMe());
		System.out.println(result);
		
		System.out.println(pairarr[0].equals(Pair.oF("A", 10)));

		}

	class compareMe implements Comparator<Integer>{

		@Override
		public int compare(Integer arg0, Integer arg1) {

			return (arg0 - arg1);
		}
		
	}

}
