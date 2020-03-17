package il.co.ilrd.filedatabase;

import java.io.IOException;

public class FileBackup {
	FileWatcher<?> fileWatcher;
	ChangeAnalyzer<?> analyzer;
	
	public FileBackup(String filePath, String backUpPath) throws Exception {
		fileWatcher = new FileWatcher<>(filePath);
		analyzer = new ChangeAnalyzer(filePath, backUpPath);
	}
	
		
}
