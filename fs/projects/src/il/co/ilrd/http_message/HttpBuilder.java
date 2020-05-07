package il.co.ilrd.http_message;

import java.util.Map;

public class HttpBuilder {
	private final static String NEW_LINE = "\r\n";
	private final static String SPACE = " ";
	private final static String HEADER_SEPERATOR = ": ";
	private final static String EMPTY_STRING = "";

	public static String createHttpRequestMessage(HttpMethod method, HttpVersion version, String url, Map<String, String> header, String body){
		return
			StartLineBuilder.createStartLineRequest(method, version, url) +
			HeaderBuilder.createHeader(header) + 
			NEW_LINE +
			BodyBuilder.createBody(body);
	}
	
	public static String createHttpResponseMessage(HttpVersion version, HttpStatusCode code, Map<String, String> header, String body){
		return
			StartLineBuilder.createStartLineResponse(version, code) +
			HeaderBuilder.createHeader(header) +
			NEW_LINE +
			BodyBuilder.createBody(body);
	}
	
	public static class StartLineBuilder {
		static String createStartLineRequest(HttpMethod method, HttpVersion version, String url) {
			return method.getMethodAsString() + SPACE + url + SPACE + version.getVersionAsString() + NEW_LINE;
		}
		
		static String createStartLineResponse(HttpVersion version, HttpStatusCode code) {
			return version.getVersionAsString() + SPACE + code.asText() + SPACE + code.getDescription() + NEW_LINE;
		}
	}
	
	public static class HeaderBuilder {
		static String createHeader(Map<String, String> header) {
			String headers = EMPTY_STRING;
			if(null != header && !header.isEmpty()) {
				for (String key: header.keySet()) {
					headers += key + HEADER_SEPERATOR + header.get(key) + NEW_LINE;
		        }
			}
			return headers;
		}
	}
	
	public static class BodyBuilder {
		static String createBody(String body) {
			if(null == body) {
				return EMPTY_STRING;
			}
			return body;
		}
	}
}