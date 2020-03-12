package test;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class javaex7 {

	public static void main(String[] args) {
		Map<String, String> premium = new HashMap<String, String>();
		premium.put("apple", "iphone");
		premium.put("htc", "htc one");
		premium.put("samsung", "s5");
		Iterator iterator = premium.keySet().iterator();
		
		while(iterator.hasNext()) {
			System.out.println(premium.get(iterator.next()));
		//	premium.put("sony", "xperia");
		}
	}

}
