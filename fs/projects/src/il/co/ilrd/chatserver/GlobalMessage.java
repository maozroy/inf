package il.co.ilrd.chatserver;

public interface GlobalMessage<K, D> {
	
	public K getKey();
	public D getData();	
}