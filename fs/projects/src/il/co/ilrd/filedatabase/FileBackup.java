package il.co.ilrd.filedatabase;

import java.io.IOException;
import java.nio.file.ClosedWatchServiceException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import static java.nio.file.StandardWatchEventKinds.ENTRY_CREATE;
import static java.nio.file.StandardWatchEventKinds.ENTRY_DELETE;
import static java.nio.file.StandardWatchEventKinds.ENTRY_MODIFY;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.Objects;

import il.co.ilrd.observer.Callback;
import il.co.ilrd.observer.Dispatcher;


public class FileBackup {
	private FileWatcher fileWatcher;
	private ChangesAnalyzer changesAnalyzer;
	private File backupFile;
	private File watchedFile;
	private final static String BACKUP = "_backup";
	
	public FileBackup(String watchedFile) throws IOException {
		checkIfFile(Path.of(Objects.requireNonNull(watchedFile)));
		this.watchedFile = new File(watchedFile);
		fileWatcher = new FileWatcherImpl(watchedFile);
	}
	
	public void stopFileBackup() throws IOException {
		changesAnalyzer.unregitser(fileWatcher);
	}
	
	public void startFileBackup() throws Exception {
		if (null == backupFile) {
			setBackupFile(getDefaultBackupName());
		}
		if (null == changesAnalyzer) {
			changesAnalyzer = new ChangesAnalyzerImpl(watchedFile, backupFile);
		}
		changesAnalyzer.register(fileWatcher);
	}
	


	public void setBackupFile(String backupFile ) throws Exception {
		if (changesAnalyzer != null) {
			fileWatcher.stopUpdate();
			throw new Exception("Backup already defined");
		}
		
		
		
		this.backupFile = new File(backupFile);
		
		if (this.backupFile.isDirectory()) {
			fileWatcher.stopUpdate();
			throw new IllegalArgumentException("Path is directory");
		}
		if (!this.backupFile.exists()) {
			if (!this.backupFile.createNewFile()) {	
				fileWatcher.stopUpdate();
				throw new IOException("Creating failed");
			}
		}
	}
	
	private String getDefaultBackupName() {
		String originalPath = watchedFile.toString();
		String path = originalPath.substring(0, originalPath.indexOf('.'));
		
		path = path + BACKUP + originalPath.substring(originalPath.indexOf('.'), originalPath.length());
		return path;
	}
	
	static void checkIfFile(Path dir) throws IOException {
		if (!dir.toFile().isFile()) {
			throw new IOException("Not a file");
		}		
	}
	public class ChangesAnalyzerImpl implements ChangesAnalyzer {
		
		private File watchedFile;
		private File backupFile;
		private Callback<Void> callback;
		private CRUDFile crudFile;
		
		public ChangesAnalyzerImpl(File watched, File backUpPath) throws IOException  {
			 crudFile = new CRUDFile(backUpPath);
			 watchedFile = watched;
			 backupFile = backUpPath;
		}
		
		@Override
		public void register(FileWatcher fileWatcher) throws IOException{
			initCallback();
			Objects.requireNonNull(fileWatcher).register(callback);
		}
		
		@Override
		public void unregitser(FileWatcher fileWatcher) {
			Objects.requireNonNull(fileWatcher).unregister(callback);
		}

		private void compareFiles(Void x) throws IOException {
			try(BufferedReader watchedBuffer = new BufferedReader(new FileReader(watchedFile));
				BufferedReader backupBuffer = new BufferedReader(new FileReader(backupFile))){
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
			}
		}
		
		private void initCallback() {
			callback = new Callback<Void>(
				arg0 -> {
					try {compareFiles(arg0);} 
					catch (IOException e) {
						e.printStackTrace(); //only initializing callback. not calling the "compare"
					}
			}, null);		
		}
	}
	
	public class FileWatcherImpl implements Runnable, FileWatcher{
		private boolean isWatching = true;
		private Dispatcher<Void> dispatcher;
		private WatchService watcher;
		private WatchKey key;
		private Path FileToWatch;
		
		public FileWatcherImpl(String filePath) throws IOException {
			dispatcher = new Dispatcher<Void>();
	        initWatchService(filePath);
	        new Thread(this).start();
		}
		
		@Override
		public void register(Callback<Void> callback) {
			dispatcher.register(Objects.requireNonNull(callback));
		}
		
		@Override
		public void unregister(Callback<Void> callback) {
			dispatcher.unregister(Objects.requireNonNull(callback));
		}
		

		
		@Override
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
		
		private void updateAll(Void event) {
			dispatcher.updateAll(event);
		}
		
		private void initWatchService(String filePath) throws IOException {
			Path dir = FileSystems.getDefault().getPath(filePath);	
			watcher = FileSystems.getDefault().newWatchService();
	        FileToWatch = dir.getFileName();
	        FileBackup.checkIfFile(dir);
	        dir.getParent().register(watcher, ENTRY_CREATE, ENTRY_DELETE, ENTRY_MODIFY);
		}

	}
	
}




