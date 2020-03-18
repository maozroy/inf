package il.co.ilrd.filedatabase;

import java.io.IOException;
import java.nio.file.ClosedWatchServiceException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import static java.nio.file.StandardWatchEventKinds.*;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.Objects;

import il.co.ilrd.observer.*;

public class FileWatcher implements Runnable{
	private boolean isWatching = true;
	private Dispatcher<Integer> dispatcher;
	private WatchService watcher;
	private WatchKey key;
	private Path FileToWatch;
	
	public FileWatcher(String filePath) throws IOException {
		dispatcher = new Dispatcher<Integer>();
        initWatchService(filePath);
        new Thread(this).start();
	}

	private void initWatchService(String filePath) throws IOException {
		Path dir = FileSystems.getDefault().getPath(filePath);	
		watcher = FileSystems.getDefault().newWatchService();
        FileToWatch = dir.getFileName();
        FileBackup.checkIfFile(dir);
        dir.getParent().register(watcher, ENTRY_CREATE, ENTRY_DELETE, ENTRY_MODIFY);
	}


	public void register(Callback<Integer> callback) {
		dispatcher.register(Objects.requireNonNull(callback));
	}
	
	public void unregister(Callback<Integer> callback) {
		dispatcher.unregister(Objects.requireNonNull(callback));
	}
	
	private void updateAll(Integer event) {
		dispatcher.updateAll(event);
	}
	
	public void stopUpdate() throws IOException {
		isWatching = false;
		watcher.close();
	}

	@Override
	public void run() {
		Path curPath;
        while (isWatching) {
		    try {
				key = watcher.take();
			    if (null != key) {
			    	 for (WatchEvent<?> event : key.pollEvents()) {
			    		 curPath = (Path)event.context();
			    		if (curPath.endsWith(FileToWatch)) {
						       updateAll(null);
						}
			    	 }
					 key.reset();
				}
			}catch (InterruptedException e ) {
				e.printStackTrace();
			}catch (ClosedWatchServiceException e) {
				//this exception caught to interrupt the blocking watcher.take() in case of closing.
			}
		}
    }


}
	
