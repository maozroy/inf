package il.co.ilrd.pingpongtcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;


public class WebPageDownload {
	URL url;
	InputStream inputStream;
	BufferedReader bufferedReader;
	String line;
	public WebPageDownload(String url) throws IOException {
		this.url = new URL(url);
		inputStream = this.url.openStream();
		bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
		while ((line = bufferedReader.readLine()) != null) {
			System.out.println(line + "\n");
		}
	}

}
