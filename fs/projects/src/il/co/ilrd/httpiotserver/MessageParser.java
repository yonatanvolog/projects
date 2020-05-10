package il.co.ilrd.httpiotserver;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;

import il.co.ilrd.http_message.HttpMethod;
import il.co.ilrd.http_message.HttpParser;
import il.co.ilrd.http_message.HttpVersion;

public class MessageParser {
	private HttpParser httpParser;
	private UrlParser urlParser;
	private JSONObject jason;
	
	public MessageParser(String message) throws Exception {
		httpParser = new HttpParser(message);
		urlParser = new UrlParser(httpParser.getStartLine().getURL());
		String body = httpParser.getBody().getBody();
		if(null != body) {
			jason = new JSONObject(httpParser.getBody().getBody());
		}
	}

	//Http message parameters
	public String get(UrlKeys databaseName) {
		return urlParser.get(databaseName);
	}
	
	public HttpVersion getVersion() {
		return httpParser.getStartLine().getHttpVersion();
	}
	
	public HttpMethod getHttpMethod() {
		return httpParser.getStartLine().getHttpMethod();
	}
	
	public DatabaseKeys getDatabaseMethod() {
		String databaseMethodAsString = get(UrlKeys.METHOD);
		for (DatabaseKeys key : DatabaseKeys.values()) {
			if(key.toString().equals(databaseMethodAsString)) {
				return key;
			}
        }
		
		return null;
	}

	//Url/Json parameters
	public String get(String param) {
		String paramInUrl = urlParser.get(param);
		if(null != paramInUrl) {
			return paramInUrl;
		}
		
		if(null != jason) {
			return (String) jason.get(param);			
		}
		
		return null;
	}
	
	public static boolean isOptions(String message) {
		String tokens[] = message.split("\r\n");

		return tokens[0].startsWith("OPTIONS / HTTP/");
	}
	
	private class UrlParser {
		Map<String, String> parametersMap = null;
		
		public UrlParser(String url) {
			initMap(url);
		}
		
		public String get(String key) {
			return parametersMap.get(key);
		}
		
		public String get(UrlKeys key) {
			return parametersMap.get(key.asString());
		}
		
		private void initMap(String url) {
			if(null != url && !url.equals("")) {
				parametersMap = new HashMap<>();			
				url = url.substring(1);
				String[] tokens = url.split("/", 3);
				
				parametersMap.put(UrlKeys.DATABASE_NAME.asString(), tokens[0]);
				parametersMap.put(UrlKeys.TABLENAME.asString(), tokens[1]);

				tokens = tokens[2].split("\\?", 2);
				parametersMap.put(UrlKeys.METHOD.asString(), tokens[0]);
				
				if(tokens.length > 1) {
					String[] pairs = tokens[1].split("&");
					for (String value : pairs) {
						final String PAIR_DELIMITER = "=";					
						String[] pair = value.split(PAIR_DELIMITER);
						parametersMap.put(pair[0], pair[1]);
					}
				}
			}
		}
	}

}