package il.co.ilrd.http_message;

import java.net.MalformedURLException;
import java.util.Map;

public class Test {

	public static void main(String[] args) throws MalformedURLException {
		HttpParser parsed = new HttpParser("GET /hello.htm HTTP/1.1 \n"
				+ " User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT  \n"
				+ "Host: www.tutorialspoint.com\n" + 
				"Accept-Language: en-us\n" + 
				"Accept-Encoding: gzip, deflate\n" + 
				"Connection: Keep-Alive \n");
		
		System.out.println(parsed.getStartLine().getURL());
		Map<?, ?> header = parsed.getHeader().getAllHeaders();
		for (Object entry : header.values()) {
			System.out.println(entry);
		}
		System.out.println(parsed.getStartLine().getHttpMethod());
		System.out.println(parsed.getStartLine().getHttpVersion());

		
	}

}
