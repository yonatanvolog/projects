package il.co.ilrd.sunhttpserver;

import java.net.URI;
import java.util.HashMap;
import java.util.Map;

class UriParser {
	public static String getDbName(URI uri) throws BadRequestException {
		String tokens[];
		try {
			String rawUri = uri.getRawPath();
			tokens = rawUri.split("/", 4);
		} catch (Exception e) {
			throw new BadRequestException();
		}
	
		return tokens[2];
	}
	
	public static String getTableName(URI uri) throws BadRequestException {
		String tokens[];
		try {
			String rawUri = uri.getRawPath();
			tokens = rawUri.split("/", 4);
		} catch (Exception e) {
			throw new BadRequestException();
		}

		return tokens[3];
	}

	public static Map<String,String> getQueryMap(URI uri) throws BadRequestException {
		Map<String,String> parametersMap;
		try {
			String rawUri = uri.getRawQuery();
			parametersMap = new HashMap<>();
			String[] pairs = rawUri.split("&");
			String[] pair;
			for (String value : pairs) {
				final String PAIR_DELIMITER = "=";					
				pair = value.split(PAIR_DELIMITER);
				parametersMap.put(pair[0], pair[1]);
			}
		} catch (Exception e) {
			throw new BadRequestException();
		}
		
		return parametersMap;
	}
}