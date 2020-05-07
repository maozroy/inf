package il.co.ilrd.server.chatserver;

public interface Message<K, V> {
	
	public K getKey();
	public V getData();
	
}