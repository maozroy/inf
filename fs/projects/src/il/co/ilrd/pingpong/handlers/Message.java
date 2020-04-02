package il.co.ilrd.pingpong.handlers;

interface Message<K, V> {
	
	public K getKey();
	public V getData();
	
}