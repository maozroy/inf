package il.co.ilrd.http_message;

import java.util.Map;
import java.util.HashMap;

public class HttpParser {
	private static final String CRLF = "\\r\\n";
	
	private StartLine startLine;
	private Header header;
	private Body body;
	
	public HttpParser(String message) {
		String[] lines = message.split(CRLF);
		int headerEnd = GetEndOfHeader(lines);
		
		if (0 != headerEnd) {
			header = new Header(lines, headerEnd);			
		}
		
		startLine = new StartLine(lines[0]);
		
		if (HasBody(headerEnd, lines)) {
			body = new Body(lines, headerEnd);			
		}
	}

	public StartLine getStartLine() {
		return startLine;
	}

	public Header getHeader() {
		return header; 
	}

	public Body getBody() {
		return body;
	}

	public boolean isRequest() {
		return startLine.isRequest();
	}
	
	public boolean isReply() {
		return startLine.isReply();
	}
	
	private int GetEndOfHeader(String[] lines) {
		int i = 0;
		for (i = 0; i < lines.length; i++) {
			if (lines[i].trim().isEmpty()) {
				break;
			}
		}
		return i;
	}
	
	private boolean HasBody(int headerEnd, String[] lines) {
		return (lines.length > headerEnd + 1);
	}

	
	public class StartLine {
		private static final int REQ_VERSION_POS = 2; 
		private static final int RES_VERSION_POS = 0;
		private static final int RES_STATUS_CODE_POS = 1;
		private static final int REQ_URL_POS = 1;
		private static final int REQ_METHOD_POS = 0;
		private static final String SP = " ";


		private HttpVersion version;
		private HttpMethod method;
		private HttpStatusCode status;
		private String url;
		
		private StartLine(String startLine){
			String[] lines = startLine.split(SP);
			version = getVersion(lines[RES_VERSION_POS]);
			if (null == version) {
				HandleRequest(lines); 
			}else {
				HandleResoponse(lines);
			}
		}
		
		public HttpMethod getHttpMethod() {
			return method;
		}
		
		public String getURL() {
			return url;
		}
		
		public HttpStatusCode getStatus() {
			return status;
		}
		
		public boolean isRequest() {
			return (null != method);
		}
		
		public boolean isReply() {
			return (null != status);
		}
		
		public HttpVersion getHttpVersion() {
			return version;
		}		
		
		private HttpVersion getVersion(String versionString) {
			for (HttpVersion version : HttpVersion.values()) {
				if (version.getVersionAsString().equals(versionString)) {
					return version;
				}
			}
			return null;
		}

		private HttpMethod GetMethodEnum(String methodString) {
			for (HttpMethod method : HttpMethod.values()) {
				if (method.getMethodAsString().equals(methodString)) {
					return method;
				}
			}
			return null;
		}

		private void HandleResoponse(String[] lines) {
			
			status = GetStatusEnum(Integer.parseInt(lines[RES_STATUS_CODE_POS]));
			version = getVersion(lines[RES_VERSION_POS]);
		}

		private HttpStatusCode GetStatusEnum(int msgCode) {
			for (HttpStatusCode code : HttpStatusCode.values()) {
				if (code.getCode() == msgCode) {
					return code;
				}
			}			
			return null;
		}

		private void HandleRequest(String[] lines){
			url = lines[REQ_URL_POS];
			method = GetMethodEnum(lines[REQ_METHOD_POS]);
			version = getVersion(lines[REQ_VERSION_POS]);
		}
	}
	
	public class Header {
		private static final String HEADER_DELIMITER = ": ";
		private HashMap<String, String> headerMap = new HashMap<>();
		
		private Header(String[] headers, int headerEnd) {
			for (int i = 1; i < headerEnd; i++) {
				String[] lines = headers[i].split(HEADER_DELIMITER);
				headerMap.put(lines[0], lines[1]);
			}
		}
	
		public String getHeader(String header) {
			return headerMap.get(header);
		}
	
		public Map<String, String> getAllHeaders() {
			return headerMap;
		}
	}
	
	public class Body {
		private String body = "";
	
		private Body(String[] lines, int headerEnd) {
			for (int i = headerEnd + 1; i < lines.length; i++) {
				body += lines[i];
			}
		}

		public String getBody() {
			return body;
		}
	}
}