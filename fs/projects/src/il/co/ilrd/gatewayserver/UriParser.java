package il.co.ilrd.gatewayserver;

import java.net.URI;
import java.util.HashMap;
import java.util.Map;

class BadRequestException extends RuntimeException {
	private static final long serialVersionUID = 1L;
}

class UriParser {
	final static int DB_INDEX = 2;
	final static int TABLE_INDEX = 3;
	final static int NUM_OF_PARTS_IN_URI = 4;

	public static String getDbName(URI uri) throws BadRequestException {
		String tokens[];
		try {
			String rawUri = uri.getRawPath();
			tokens = rawUri.split("/", NUM_OF_PARTS_IN_URI);
		} catch (Exception e) {
			throw new BadRequestException();
		}
	
		return tokens[DB_INDEX];
	}
	
	public static String getTableName(URI uri) throws BadRequestException {
		String tokens[];
		try {
			String rawUri = uri.getRawPath();
			tokens = rawUri.split("/",NUM_OF_PARTS_IN_URI);
		} catch (Exception e) {
			throw new BadRequestException();
		}

		return tokens[TABLE_INDEX];
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