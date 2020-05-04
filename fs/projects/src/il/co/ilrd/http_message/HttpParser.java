package il.co.ilrd.http_message;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

public class HttpParser {
	private StartLineParser startLine;
	private HeaderParser header;
	private BodyParser body;
	private final static String NEW_LINE = "\r\n";
	private final static String EMPTY_LINE = NEW_LINE + NEW_LINE; 
	private final static String HTTP_PREFIX = "http://";
	private final static String EMPTY_STRING = "";

	public HttpParser(String message) throws MalformedURLException {	
		header = new HeaderParser(message);	
		startLine = new StartLineParser(message);	
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
		int indexOfStartLineEnd = message.indexOf(NEW_LINE);
		
		return indexOfStartLineEnd + NEW_LINE.length();
	}
	
	private static int getFirstIndexOfBody(String message) {
		int indexOfHeadersEnd = 
				message.indexOf(EMPTY_LINE, getFirstIndexOfHeaders(message));
		
		return indexOfHeadersEnd + EMPTY_LINE.length();
	}

	private static boolean isResponse(String message) {
		if(message.startsWith("HTTP")) {
			return true;
		}
		return false;
	}
	
	private static boolean isRequest(String message) {
		return !isResponse(message);
	}
	
	public class StartLineParser {
		HttpMethod method;
		HttpVersion version;
		HttpStatusCode status;
		URL url;
		
		public StartLineParser(String message) throws MalformedURLException {
			this.method = getMethodFromHttpMessage(message);
			this.version = getVersionFromHttpMessage(message);
			this.status = getStatusFromHttpMessage(message);
			this.url = getUrlFromHttpMessage(message);
		}
		
		public HttpMethod getHttpMethod() {
			return method;
		}
		
		public URL getURL() {
			return url;
		}
		
		public HttpStatusCode getStatus() {
			return status;
		}
		
		public boolean isRequest() {
			if(null != method && null == status) {
				return true;
			}
			
			return false;
		}
		
		public boolean isReply() {
			return !isRequest();
		}
		
		public HttpVersion getHttpVersion() {
			return version;
		}
		
		private HttpMethod getMethodFromHttpMessage(String message) {
			if(HttpParser.isRequest(message)) {
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
		
		private URL getUrlFromHttpMessage(String message) throws MalformedURLException {
			URL url = null;

			if(HttpParser.isRequest(message)) {
				final String SPACE_DELIMITER = " ";
		        String[] tokens = getStartLineSubstring(message).split(SPACE_DELIMITER);  
				
				if(!tokens[1].startsWith(HTTP_PREFIX)) {
					if(null == header.getHeader("Host")) {
						throw new MalformedURLException("Missing host header");
					}
					url = new URL(HTTP_PREFIX + header.getHeader("Host") + tokens[1]);
				}
				else {
					url = new URL(tokens[1]);
				}
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
			String[] startLineAndHeaders = message.split(NEW_LINE, 2);

			return !startLineAndHeaders[1].startsWith(NEW_LINE);
		}

		private Map<String, String> createMapOfHeaders(String message) {
			String headers = getHeadersSubstring(message);
			Map<String, String> mapOfHeaders = new HashMap<>();
			
			if(!headers.equals(EMPTY_STRING)) {
				String[] rows = headers.split(NEW_LINE);  
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
			String[] secondSplitResult = message.split(NEW_LINE + NEW_LINE, 2);
			
			if (secondSplitResult[1].length() > 0) {
				return secondSplitResult[1];
			}
			
			return null;
		}
	}
}