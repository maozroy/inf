package il.co.ilrd.observer;

import java.util.function.Consumer;

public class Observer <T> {
	
	private Callback<T> callback;
	
	public void regitser(Subject<T> subject) {
		Consumer<T> myUpdate = (param) -> {System.out.println("UPDATE " + param);};
		Consumer<T> myStopUpdate = (param) -> {System.out.println("STOP UPDATE");};
		callback = new Callback<T>(myUpdate , myStopUpdate);
		subject.register(callback);
		
	}
	public void unregitser() {
		callback.getDispatcher().unregister(callback);
	}	
	public void unregitser(Subject<T> subject) {
		subject.unregister(callback);
	}
	
}
