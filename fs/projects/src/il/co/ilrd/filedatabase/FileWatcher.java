package il.co.ilrd.filedatabase;

import java.io.File;
import java.io.IOException;
import java.nio.file.ClosedWatchServiceException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.Paths;
import static java.nio.file.StandardWatchEventKinds.*;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;

import il.co.ilrd.observer.*;

public class FileWatcher<T> implements Runnable{
	private boolean isAlive = true;
	private Dispatcher<WatchEvent<?>> dispatcher;
	private WatchService watcher;
	WatchKey key;
	Thread thread;

	public FileWatcher(String filePath) throws IOException, InterruptedException {
		dispatcher = new Dispatcher<WatchEvent<?>>();
		watcher = FileSystems.getDefault().newWatchService();
        Path dir = FileSystems.getDefault().getPath(filePath);
        key = dir.register(watcher, ENTRY_CREATE, ENTRY_DELETE, ENTRY_MODIFY);
       
        new Thread(this).start();
		
	}

	public void register(Callback<WatchEvent<?>> callback) {
		dispatcher.register(callback);
	}
	
	public void unregister(Callback<WatchEvent<?>> callback) {
		dispatcher.unregister(callback);
	}
	
	private void updateAll(WatchEvent<?> event) {
		dispatcher.updateAll(event);
	}
	
	public void stopUpdate() throws IOException {
		isAlive = false;
		watcher.close();
		
	}

	@Override
	public void run() {

        while (isAlive) {
		    try {
				key = watcher.take();

			    if (key != null) {
			    	 for (WatchEvent<?> event : key.pollEvents()) {
					       updateAll(event);
					    }
					    key.reset();
				}
			} catch (InterruptedException e ) {
				e.printStackTrace();
			}catch (ClosedWatchServiceException e) {
					
			}
		}
    }
}
	
