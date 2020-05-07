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
			
			if (secondSplitResult[1].trim().length() > 0) {
				return secondSplitResult[1];
			}
			
			return null;
		}
	}
}

//package il.co.ilrd.http_message;
//
//import java.net.MalformedURLException;
//import java.util.HashMap;
//import java.util.Map;
//
//public class HttpParser {
//	private StartLineParser startLine;
//	private HeaderParser header;
//	private BodyParser body;
//	private static final String NEW_LINE = "\r\n";
//	private static final int NUM_OF_PARAMS_IN_STARTLINE = 3;
//	private static final int SPLIT_INTO_TWO_PARTS = 2;
//	private static final String HTTP = "http://";
//	private static final String HOST = "Host";
//	private static final String SPACE = " ";
//	private static final String COLON = ": ";
//	
//	
//	public HttpParser(String message) {
//		String[] firstSplitResult = message.split(NEW_LINE, SPLIT_INTO_TWO_PARTS);
//		
//		String[] secondSplitResult = firstSplitResult[1].split(NEW_LINE + NEW_LINE, SPLIT_INTO_TWO_PARTS);
//		if (secondSplitResult[0].length() > 0) {
//			header = new HeaderParser(secondSplitResult[0]);
//		}
//		
//		startLine = new StartLineParser(firstSplitResult[0]);
//		
//		if (secondSplitResult[1].length() > 0) {
//			body = new BodyParser(secondSplitResult[1]);
//		}
//	}
//	
//	public StartLineParser getStartLine() {
//		return startLine;
//	}
//
//	public HeaderParser getHeader() {
//		return header;
//	}
//
//	public BodyParser getBody() {
//		return body;
//	}
//
//	boolean isRequest() {
//		return startLine.isRequest();
//	}
//	
//	boolean isReply() {
//		return startLine.isReply();
//	}
//	
//	public class StartLineParser {
//		private HttpVersion version;
//		private HttpMethod method;
//		private HttpStatusCode status;
//		private String url;
//		
//		public StartLineParser(String startLine) {
//			String[] SplitResult = startLine.split(SPACE, NUM_OF_PARAMS_IN_STARTLINE);
//			method = findMatchingMethod(SplitResult[0]);
//				
//			if(isReply()) {
//				version = findMatchingVersion(SplitResult[0]);
//				status = findMatchingStatus(SplitResult[1], SplitResult[2]);
//			}
//				
//			else {
//				version = findMatchingVersion(SplitResult[2]);
//				
//				url = SplitResult[1];
//
//			}
//		}
//		
//		public HttpMethod getHttpMethod() {
//			return method;
//		}
//		
//		public String getURL() {
//			return url;
//		}
//		
//		public HttpStatusCode getStatus() {
//			return status;
//		}
//		
//		public boolean isRequest() {
//			return null != method;
//		}
//		
//		public boolean isReply() {
//			return null == method;
//		}
//		
//		public HttpVersion getHttpVersion() {
//			return version;
//		}
//		
//		private HttpMethod findMatchingMethod(String httpMethod) {
//			for(HttpMethod method : HttpMethod.values()) {
//				if (method.getMethodAsString().equals(httpMethod)) {
//					return method;
//				}
//			}
//			
//			return null;
//		}
//		
//		private HttpVersion findMatchingVersion(String versionString) {
//			for(HttpVersion version : HttpVersion.values()) {
//				if (version.getVersionAsString().equals(versionString)) {
//					return version;
//				}
//			}
//			
//			return null;
//		}
//		
//		private HttpStatusCode findMatchingStatus(String code, String description) {
//			for(HttpStatusCode status : HttpStatusCode.values()) {
//				if ((status.asText().equals(code)) && (status.getDescription().equals(description))) {
//					return status;
//				}
//			}
//			
//			return null;
//		}
//	}
//	
//	public class HeaderParser {
//		private Map<String, String> headersMap = new HashMap<>();
//		
//		public HeaderParser(String headersString) {
//			String[] headersArray = headersString.split(NEW_LINE);
//			for (String singleHeader : headersArray) {
//				String[] splitHeader = singleHeader.split(COLON, SPLIT_INTO_TWO_PARTS);
//				headersMap.put(splitHeader[0], splitHeader[1]);
//			}
//		}
//	
//		public String getHeader(String header) {
//			return headersMap.get(header);
//		}
//	
//		public Map<String, String> getAllHeaders() {
//			return headersMap;
//		}
//	}
//	
//	public class BodyParser {
//		private String body;
//		
//		public BodyParser(String body) {
//			this.body = body;
//		}
//	
//		public String getBody() {
//			return body;
//		}
//	}
//}