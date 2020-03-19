package il.co.ilrd.filedatabase;

import java.io.IOException;

public interface ChangesAnalyzer {
	public void register(FileWatcher fileWatcher) throws IOException;
	public void unregitser(FileWatcher fileWatcher);
}
