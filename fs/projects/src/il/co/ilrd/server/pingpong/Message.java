package il.co.ilrd.server.pingpong;

public interface Message<K, V> {
	
	public K getKey();
	public V getData();
	
}