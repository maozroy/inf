package il.co.ilrd.server.general;

public interface Message<K, V> {
	
	public K getKey();
	public V getData();
	
}