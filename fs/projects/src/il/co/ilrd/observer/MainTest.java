package il.co.ilrd.observer;

public class MainTest {

	public static void main(String[] args) {
		Observer<String> observer = new Observer<>();
		Subject<String> subject = new Subject<String>("DAMN");
		observer.regitser(subject);
		subject.updateAll();
		observer.unregitser();
		subject.updateAll();
		
	}

}
