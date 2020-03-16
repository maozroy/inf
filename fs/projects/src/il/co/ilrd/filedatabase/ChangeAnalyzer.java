package il.co.ilrd.filedatabase;

import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.util.function.Consumer;
import static java.nio.file.StandardWatchEventKinds.*;


import il.co.ilrd.observer.Callback;

public class ChangeAnalyzer <T> {
	
	private Callback<WatchEvent<?>> callback;
	
	public ChangeAnalyzer(String filePath, String backUpPath) {
		// TODO Auto-generated constructor stub
	}
	public void regitser(FileWatcher<WatchEvent<?>> fileWatcher) {
		Consumer<WatchEvent<?>> myUpdate = (param) -> {
			WatchEvent.Kind<?> kind = param.kind();
			if (kind.equals(ENTRY_CREATE)){
				System.out.println("entry created");
			}
             else if (ENTRY_MODIFY.equals(kind)) {
                System.out.println("Entry was modified on log dir.");
            } else if (ENTRY_DELETE.equals(kind)) {
                System.out.println("Entry was deleted from log dir.");
            }
		};
		callback = new Callback<WatchEvent<?>>(myUpdate , null);
		fileWatcher.register(callback);
		
	}
	public void unregitser() {
		callback.getDispatcher().unregister(callback);
	}	
	public void unregitser(FileWatcher<StandardWatchEventKinds> fileWatcher) {
		fileWatcher.unregister(callback);
	}
	
}
