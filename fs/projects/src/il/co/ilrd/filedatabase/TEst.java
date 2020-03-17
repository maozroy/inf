package il.co.ilrd.filedatabase;

import java.io.IOException;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.util.function.Consumer;

import il.co.ilrd.observer.Callback;
import il.co.ilrd.observer.Subject;

public class TEst {

	public static void main(String[] args) throws Exception {
		FileWatcher<WatchEvent<?>> fileWatcher = new FileWatcher<WatchEvent<?>>("/home/maoz/test.txt");
		ChangeAnalyzer<WatchEvent<?>> observer =new ChangeAnalyzer(null, null);
		observer.regitser(fileWatcher);
		Thread.sleep(5000);
		System.out.println("end");
	//	fileWatcher.stopUpdate();
		
	}

}
