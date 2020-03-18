package il.co.ilrd.filedatabase;

import java.io.IOException;
import java.nio.file.Path;

public class FileBackup {
	FileWatcher fileWatcher;
	ChangesAnalyzer analyzer;
	
	public FileBackup(String filePath, String backUpPath) throws Exception {
		fileWatcher = new FileWatcher(filePath);
		analyzer = new ChangesAnalyzer(filePath, backUpPath);
	}
	
		
	
	static void checkIfFile(Path dir) throws IOException {
		if (!dir.toFile().isFile()) {
			throw new IOException("Not a file");
		}		
	}
}
