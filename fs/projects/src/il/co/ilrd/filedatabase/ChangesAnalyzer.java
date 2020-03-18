package il.co.ilrd.filedatabase;

import java.nio.file.Paths;
import java.util.Objects;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import il.co.ilrd.observer.Callback;

public class ChangesAnalyzer  {
	
	private File watchedFile;
	private File backupFile;
	private Callback<Integer> callback;
	private CRUDFile crudFile;
	
	public ChangesAnalyzer(String filePath, String backUpPath) throws IOException {
		 FileBackup.checkIfFile(Paths.get(filePath));
		 FileBackup.checkIfFile(Paths.get(backUpPath));
		 crudFile = new CRUDFile(backUpPath);
		 watchedFile = new File(filePath);
		 backupFile = new File(backUpPath);
	}
	
	public void register(FileWatcher fileWatcher) throws IOException{
		initCallback();
		Objects.requireNonNull(fileWatcher).register(callback);
	}

	private void compareFiles(Integer integer) throws IOException {
		BufferedReader watchedBuffer = new BufferedReader(new FileReader(watchedFile));
		BufferedReader backupBuffer = new BufferedReader(new FileReader(backupFile));

		String lineBackup = backupBuffer.readLine();
		String lineWatched = watchedBuffer.readLine();
		
		while(null != lineWatched) {
			if((null == lineBackup)) {
				crudFile.create(lineWatched);
				lineBackup = backupBuffer.readLine();
			}
			lineBackup = backupBuffer.readLine();
			lineWatched = watchedBuffer.readLine();
		}
		watchedBuffer.close();
		backupBuffer.close();
	}
	
	public void regitser(FileWatcher fileWatcher) {
		Objects.requireNonNull(fileWatcher).register(callback);
	}
	
	public void unregitser(FileWatcher fileWatcher) {
		Objects.requireNonNull(fileWatcher).unregister(callback);
	}
	
	private void initCallback() {
		callback = new Callback<Integer>(
			arg0 -> {
				try {compareFiles(arg0);} 
				catch (IOException e) {
					e.printStackTrace(); //only initializing callback. not calling the "compare"
					}
			}, null);		
	}
}
