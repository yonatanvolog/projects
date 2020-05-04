package il.co.ilrd.http_message;

import java.util.HashMap;
import java.util.Map;

public class HttpParser {
	private StartLineParser startLine;
	private HeaderParser header;
	private BodyParser body;
	private final static String CRLF = "\r\n";
	private final static String EMPTY_LINE = CRLF + CRLF; 
	private final static String EMPTY_STRING = "";

	public HttpParser(String message) {	
		startLine = new StartLineParser(message);	
		header = new HeaderParser(message);	
		body = new BodyParser(message);
	}
	
	public StartLineParser getStartLine() {
		return startLine;
	}

	public HeaderParser getHeader() {
		return header;
	}

	public BodyParser getBody() {
		return body;
	}

	public boolean isRequest() {
		return startLine.isRequest();
	}
	
	public boolean isReply() {
		return startLine.isReply();
	}
	
	private static int getFirstIndexOfHeaders(String message) {
		int indexOfStartLineEnd = message.indexOf(CRLF);
		
		return indexOfStartLineEnd + CRLF.length();
	}
	
	private static int getFirstIndexOfBody(String message) {
		int indexOfHeadersEnd = 
				message.indexOf(EMPTY_LINE, getFirstIndexOfHeaders(message));
		
		return indexOfHeadersEnd + EMPTY_LINE.length();
	}
	
	public class StartLineParser {
		HttpMethod method;
		HttpVersion version;
		HttpStatusCode status;
		String url;
		boolean isRequest;
		boolean isReply;
		
		public StartLineParser(String message) {
			isRequest = isRequestInit(message);
			isReply = isReplyInit(message);
			this.method = getMethodFromHttpMessage(message);
			this.version = getVersionFromHttpMessage(message);
			this.status = getStatusFromHttpMessage(message);
			this.url = getUrlFromHttpMessage(message);
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
			return isRequest;
		}
		
		public boolean isReply() {
			return isReply;
		}
		
		public HttpVersion getHttpVersion() {
			return version;
		}
		
		private boolean isReplyInit(String message) {
			if(message.startsWith("HTTP")) {
				return true;
			}
			return false;
		}
		
		private boolean isRequestInit(String message) {
			return !isReplyInit(message);
		}
		
		private HttpMethod getMethodFromHttpMessage(String message) {
			if(isRequest) {
				for(HttpMethod method : HttpMethod.values()) {
					if(message.startsWith(method.getMethodAsString())) {
						return method;
					}
				}
			}
			
			return null;
		}
		
		private HttpVersion getVersionFromHttpMessage(String message) {
			String startLine = getStartLineSubstring(message);
			
			for(HttpVersion version : HttpVersion.values()) {
				if(startLine.contains(version.getVersionAsString())) {
					return version;
				}
			}
			
			return null;
		}
		
		private HttpStatusCode getStatusFromHttpMessage(String message) {
			String startLine = getStartLineSubstring(message);
			
			for(HttpStatusCode code : HttpStatusCode.values()) {
				if(startLine.contains(code.asText())) {
					return code;
				}
			}
			
			return null;
		}
		
		private String getUrlFromHttpMessage(String message) {
			String url = null;

			if(isRequest) {
				final String SPACE_DELIMITER = " ";
		        String[] tokens = getStartLineSubstring(message).split(SPACE_DELIMITER); 
				url = tokens[1];
			}
			
			return url;
		}
		
		private String getStartLineSubstring(String message) {
			int beginIndex = 0;
			int endIndex = getFirstIndexOfHeaders(message);
			
			return message.substring(beginIndex, endIndex);
		}
	}
	
	public class HeaderParser {
		Map<String, String> headers;
		
		public HeaderParser(String message) {
			Map<String, String> headers = null;
			if(includesHeaders(message)) {			
				headers = createMapOfHeaders(message);
			}
			this.headers = headers;
		}
	
		public String getHeader(String header) {
			if(null != headers) {
				return headers.get(header);				
			}
			return null;
		}
	
		public Map<String, String> getAllHeaders() {
			return headers;
		}
		
		private boolean includesHeaders(String message) {
			String[] startLineAndHeaders = message.split(CRLF, 2);

			return !startLineAndHeaders[1].startsWith(CRLF);
		}

		private Map<String, String> createMapOfHeaders(String message) {
			String headers = getHeadersSubstring(message);
			Map<String, String> mapOfHeaders = new HashMap<>();
			
			if(!headers.equals(EMPTY_STRING)) {
				String[] rows = headers.split(CRLF);  
				for (String row : rows) {
					final String PAIR_DELIMITER = ": ";					
					String[] pair = row.split(PAIR_DELIMITER);
					mapOfHeaders.put(pair[0], pair[1]);
				}
			}
			
			return mapOfHeaders;
		}
		
		private String getHeadersSubstring(String message) {
			int beginIndex = getFirstIndexOfHeaders(message);
			int endIndex = getFirstIndexOfBody(message);
				
			return message.substring(beginIndex, endIndex);
		}
	}
	
	public class BodyParser {
		String body;
		
		public BodyParser(String message) {
			this.body = getBodyFromHttpMessage(message);
		}
	
		public String getBody() {
			return body;
		}
		
		private String getBodyFromHttpMessage(String message) {
			String[] secondSplitResult = message.split(CRLF + CRLF, 2);
			
			if (secondSplitResult[1].length() > 0) {
				return secondSplitResult[1];
			}
			
			return null;
		}
	}
}