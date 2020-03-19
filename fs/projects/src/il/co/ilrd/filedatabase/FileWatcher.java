package il.co.ilrd.filedatabase;

import java.io.IOException;

import il.co.ilrd.observer.Callback;

public interface FileWatcher {
	public void register(Callback<Void> callback);
	public void unregister(Callback<Void> callback);
	public void stopUpdate() throws IOException;
}
