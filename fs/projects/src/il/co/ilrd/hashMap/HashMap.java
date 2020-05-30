package il.co.ilrd.hashMap;
import java.util.AbstractCollection;
import java.util.AbstractSet;
import java.util.ArrayList;
import java.util.Collection;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import il.co.ilrd.pair.Pair;

public class HashMap<K,V> implements Map<K, V> {

	private List<List<Pair<K, V>>> hash;
	private final int capacity;
	private static final int DEFAULT_CAPACITY = 16;
	private Collection<V> valuesCollection;
	private Set<K> keyCollection;
	private Set<Map.Entry<K, V>> setCollection;
	private int modCounter = 0;


	//----------------------------------//
	
	public HashMap(int capacity) {
		this.capacity = capacity;
		initList();
	}

	public HashMap() {
		this(DEFAULT_CAPACITY);
	}
	
	private void initList() {
		hash = new ArrayList<List<Pair<K,V>>>(capacity);
	
		for(int i = 0 ; i < capacity ; i++) { 
			hash.add(i, new ArrayList<Pair<K, V>>()); 
		}
	}

	//----------------------------------//
	
	@Override
	public void clear() {
		++modCounter;
		for(List<Pair<K, V>> bucket : hash) {
			bucket.clear();
		}
	}

	@Override
	public boolean containsKey(Object arg0) {
		return (null == get(arg0)) ? false : true;
	}

	@Override
	public boolean containsValue(Object arg0) {
		for(V value : values()) {
			if(value.equals(arg0)) {
				return true;
			}
		}
		
		return false;
	}
	
	@Override
	public boolean isEmpty() {
		for(List<Pair<K, V>> bucket : hash) {
			if(!bucket.isEmpty()) {
				return false;
			}
		}
		return true;
	}
	
	@Override
	public V get(Object arg0) {
		Pair<K, V> entry = getPairByKey(arg0);
		if(null == entry) {
			return null;
		}
		return entry.getValue();
	}
	
	@Override
	public V remove(Object arg0) {
		Pair<K, V> entry = getPairByKey(arg0);
		if(null == entry) {
			return null;
		}
		V oldVal = entry.getValue();
		hash.get(GetBucket(arg0)).remove(entry);
		++modCounter;
		return oldVal;
	}

	@Override
	public V put(K arg0, V arg1) {
		int bucket = GetBucket(arg0);
		
		for(Pair<K, V> pair : hash.get(bucket)) {
			if(pair.getKey().equals(arg0)) {
				V oldVal = pair.getValue();
				pair.setValue(arg1);
				++modCounter;

				return oldVal;
			}
		}
		hash.get(bucket).add(Pair.oF(arg0, arg1));
		++modCounter;
		return null;
	}
	
	@Override
	public void putAll(Map<? extends K, ? extends V> arg0) {
		for(Map.Entry<? extends K, ? extends V> entryz : 
			arg0.entrySet()) {
				put(entryz.getKey(), entryz.getValue()); 
			}
		++modCounter;
	}

	@Override
	public int size() {
		int size = 0;
		
		for(List<Pair<K, V>> list : hash) {
			size += list.size();
		}
		return size;
	}
	
	private int GetBucket(Object arg0) {
		if (null == arg0) {
			return 0;
		}
		return arg0.hashCode() % capacity;
	}
	
	//----------------------------------//

	@Override
	public Set<Map.Entry<K, V>> entrySet() {
		if (null == setCollection) {
			setCollection = new EntrySet();
		}
		return setCollection;
	}

	@Override
	public Set<K> keySet() {
		if (null == keyCollection) {
			keyCollection = new KeyIterator();
		}
		return keyCollection;
	}

	@Override
	public Collection<V> values() {
		if (null == valuesCollection) {
			valuesCollection= new ValueIterator();
		}
		return valuesCollection;
	}
	
	private Pair<K, V> getPairByKey(Object key) {
		for(Pair<K, V> pair : hash.get(GetBucket(key))) {
			if(pair.getKey() == key || pair.getKey().equals(key)) {
				return pair;
			}
		}
		return null;
	}
	
	private class KeyIterator 
			extends AbstractSet<K> implements Iterator<K> {
		Iterator<Entry<K, V>> iter = new EntrySet().iterator();
		
		@Override
		public boolean hasNext() {
			return (iter.hasNext());
		}

		@Override
		public K next() {
			return iter.next().getKey();
		}
		
		@Override
		public Iterator<K> iterator() {
			return new KeyIterator();
		}

		@Override
		public int size() {
			return HashMap.this.size();
		}
	}
	
	private class ValueIterator 
			extends AbstractCollection<V> implements Iterator<V> {
		
		Iterator<Map.Entry<K, V>> iter = new EntrySet().iterator();
				
		@Override
		public boolean hasNext() {
			return (iter.hasNext());
		}

		@Override
		public V next() {
			return iter.next().getValue();
		}
		@Override
		public Iterator<V> iterator() {
			return new ValueIterator();
		}

		@Override
		public int size() {
			return HashMap.this.size();
		}
	}
	
	private class EntrySet extends AbstractSet<Map.Entry<K, V>> {
		@Override
		public final Iterator<Map.Entry<K, V>> iterator() {
			return new EntryIterator();
		}
		
		@Override
		public int size() {
			return HashMap.this.size();
		}

		private class EntryIterator implements Iterator<Map.Entry<K, V>> {
			private Iterator<List<Pair<K, V>>> 
					bucketIterator = hash.iterator();
			private Iterator<Pair<K, V>> 
					pairIterator = bucketIterator.next().iterator();
			private int modCheck = modCounter;
			
			
			{ 
				FindNext(); 
			}
			
			@Override
			public boolean hasNext() {
			
				if(pairIterator.hasNext()) {
					return true;
				}
				while(bucketIterator.hasNext()) {
					if(!(bucketIterator.next().isEmpty())) {
						return true;
					}
				}
				return false;
			}

			@Override
			public Entry<K, V> next() {
				checkMod();
				Pair<K, V> entry = pairIterator.next();
				FindNext();

				return entry;
			}
			
			private void checkMod() {
				if (modCheck != modCounter) {
					throw new ConcurrentModificationException();
				}
			}
			
			private void FindNext() {
			//	pairIterator = bucketIterator.next().iterator();
				while(bucketIterator.hasNext() && !pairIterator.hasNext()) {
					pairIterator = bucketIterator.next().listIterator();
				}				
			}
		}
	}
	
}
