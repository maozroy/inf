package il.co.ilrd.observer;

public class Subject<T> {
	
	private Dispatcher<T> dispatcher = new Dispatcher<T>();
	T paramT; 
	
	public Subject(T paramT) {
		this.paramT = paramT;
	}
	public void register(Callback<T> callback) {
		dispatcher.register(callback);
	}
	public void unregister(Callback<T> callback) {
		dispatcher.unregister(callback);
	}
	public void updateAll() {
		dispatcher.updateAll(paramT);
	}
	public void stopUpdate() {
		dispatcher.stopUpdate();
	}
}
