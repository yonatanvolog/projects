package il.co.ilrd.sunhttpserver;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.URI;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONObject;

import com.sun.net.httpserver.Headers;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import il.co.ilrd.DatabaseManagement.DatabaseManagement;
import il.co.ilrd.http_message.HttpStatusCode;

class BadRequestException extends RuntimeException {
	private static final long serialVersionUID = 1L;
}

public class SunHttpServer {
	private HttpServer javaHttpServer;
	
	private Map<String, DatabaseManagement> companyDatabases = new HashMap<>();
	
	private static final String URL = "jdbc:mysql://localhost:3306/";
	private static final String USER_NAME = "root";
	private static final String PASSWORD = "";
	
	private static final String PK_NAME = "pkName";
	private static final String PK_VALUE = "pkValue";
	private static final String COLUMN_NAME = "columnName";
	private static final String COLUMN_INDEX = "columnIndex";
	private static final String SQL_COMMAND = "sqlCommand";
	private static final String RAW_DATA = "rawData";
	private static final String PRIMARY_KEY_NAME = "primaryKeyName";
	private static final String PRIMARY_KEY_VALUE = "primaryKeyValue";
	private static final String NEW_VALUE = "newValue";
	
	private static final String ROW_OPTIONS = "GET, POST, DELETE, OPTIONS";
	private static final String FIELD_NAME_OPTIONS = "GET, PUT, OPTIONS";
	private static final String FIELD_INDEX_OPTIONS = "GET, PUT, OPTIONS";
	private static final String TABLE_OPTIONS = "POST, DELETE, OPTIONS";
	private static final String IOT_EVENT_OPTIONS = "POST, OPTIONS";
	private static final String ALL_OPTIONS = "GET, POST, PUT, DELETE, OPTIONS";

	private static final String MESSAGE = "message";
	private static final String ERROR_MESSAGE = "errorMessage";
	private static final String BAD_REQUEST = "bad request";
	
	public SunHttpServer(int portNumber) throws IOException {
		javaHttpServer = HttpServer.create(new InetSocketAddress(portNumber), 0);
		javaHttpServer.setExecutor(null);
		
		javaHttpServer.createContext("/row", new RowHandler());
		javaHttpServer.createContext("/field_name", new FieldNameHandler());
		javaHttpServer.createContext("/field_index", new FieldIndexHandler());
		javaHttpServer.createContext("/table", new TableHandler());
		javaHttpServer.createContext("/iotEvent", new IotEventHandler());
		javaHttpServer.createContext("/", new AllOptions());
	}
	
	public void start() {
		javaHttpServer.start();
	}
	
	/****************************
	/row
	****************************/

	class RowHandler implements HttpHandler {
		private Map<String, HttpHandler> rowHttpMethodsHandlersMap;

		private RowHandler() {
			rowHttpMethodsHandlersMap = new HashMap<>();
	    	initRowHandlers();
		}
		
		@Override
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		rowHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				String errJson = createJsonMessage(ERROR_MESSAGE, BAD_REQUEST);
				sendJsonMessage(exchange, HttpStatusCode.BAD_REQUEST, errJson);
			}
	    }
	    
		private class ReadRow implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				URI uri = exchange.getRequestURI();
				String databaseName = UriParser.getDbName(uri);
				String tableName = UriParser.getTableName(uri);
				String primaryKeyColumnName = UriParser.getQueryMap(uri).get(PK_NAME);
				Object primaryKeyValue =  UriParser.getQueryMap(uri).get(PK_VALUE);
				List<Object> resultList = null;
				try {
					createDatabaseIfNotInMap(databaseName);
					resultList = companyDatabases.get(databaseName).readRow(tableName, primaryKeyColumnName, primaryKeyValue);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}		
				String json = createJsonMessage("rowValues", resultList);
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class CreateRow implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				JSONObject jason = getJsonBody(exchange);
				String sqlCommand = (String) jason.get(SQL_COMMAND);		
				try {
					createDatabaseIfNotInMap(databaseName);
					companyDatabases.get(databaseName).createRow(sqlCommand);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}	
				String json = createJsonMessage(MESSAGE, "row created");
				sendJsonMessage(exchange, HttpStatusCode.CREATED, json);
			}
		}
		
		private class DeleteRow implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				URI uri = exchange.getRequestURI();
				String databaseName = UriParser.getDbName(uri);
				String tableName = UriParser.getTableName(uri);
				JSONObject jason = getJsonBody(exchange);
				String primaryKeyColumnName = (String) jason.get(PRIMARY_KEY_NAME);
				Object primaryKeyValue =  (String) jason.get(PRIMARY_KEY_VALUE);				
				try {
					createDatabaseIfNotInMap(databaseName);
					companyDatabases.get(databaseName).deleteRow(tableName, primaryKeyColumnName, primaryKeyValue);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}
				String json = createJsonMessage(MESSAGE, "row deleted");
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class RowOptions implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				Headers headers = exchange.getResponseHeaders();
				headers.add("Allow", ROW_OPTIONS);
				sendJsonMessage(exchange, HttpStatusCode.OK, null);
			}
		}
		
		private void initRowHandlers() {
	    	rowHttpMethodsHandlersMap.put("GET", new ReadRow());
	    	rowHttpMethodsHandlersMap.put("POST", new CreateRow());
	    	rowHttpMethodsHandlersMap.put("DELETE", new DeleteRow());
	    	rowHttpMethodsHandlersMap.put("OPTIONS", new RowOptions());
	    	rowHttpMethodsHandlersMap.put("TRACE", new NotImplemented());
	    	rowHttpMethodsHandlersMap.put("PATCH", new NotImplemented());
	    	rowHttpMethodsHandlersMap.put("HEAD", new NotImplemented());
	    }		
	}
	
	/****************************
	/field_name
	****************************/
	
	class FieldNameHandler implements HttpHandler {
		private Map<String, HttpHandler> fieldNameHttpMethodsHandlersMap;

		private FieldNameHandler() {
			fieldNameHttpMethodsHandlersMap = new HashMap<>();
			initFieldNameHandlers();
		}
		
		@Override
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		fieldNameHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				String errJson = createJsonMessage(ERROR_MESSAGE, BAD_REQUEST);
				sendJsonMessage(exchange, HttpStatusCode.BAD_REQUEST, errJson);
			}
	    }
	    
		private class ReadFieldByName implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				URI uri = exchange.getRequestURI();
				String databaseName = UriParser.getDbName(uri);
				String tableName = UriParser.getTableName(uri);
				String primaryKeyColumnName = UriParser.getQueryMap(uri).get(PK_NAME);
				Object primaryKeyValue =  UriParser.getQueryMap(uri).get(PK_VALUE);
				String columnName = UriParser.getQueryMap(uri).get(COLUMN_NAME);
				Object result = null;			
				try {
					createDatabaseIfNotInMap(databaseName);
					result = companyDatabases.get(databaseName).readField(tableName, primaryKeyColumnName, primaryKeyValue, columnName);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}				
				String json = createJsonMessage("fieldValue", (String)result);
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class UpdateFieldByName implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				URI uri = exchange.getRequestURI();
				String databaseName = UriParser.getDbName(uri);
				String tableName = UriParser.getTableName(uri);
				JSONObject body = getJsonBody(exchange);
				String primaryKeyColumnName = (String) body.get(PRIMARY_KEY_NAME);
				Object primaryKeyValue = (String) body.get(PRIMARY_KEY_VALUE);
				String columnName = (String) body.get(COLUMN_NAME); 
				Object newValue = body.get(NEW_VALUE);			
				try {
					createDatabaseIfNotInMap(databaseName);
					companyDatabases.get(databaseName).updateField(tableName, primaryKeyColumnName, primaryKeyValue, columnName, newValue);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}			
				String json = createJsonMessage(MESSAGE, "field updated");
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class FieldNameOptions implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				Headers headers = exchange.getResponseHeaders();
				headers.add("Allow", FIELD_NAME_OPTIONS);
				sendJsonMessage(exchange, HttpStatusCode.OK, null);
			}
		}
		
		private void initFieldNameHandlers() {
			fieldNameHttpMethodsHandlersMap.put("GET", new ReadFieldByName());
			fieldNameHttpMethodsHandlersMap.put("PUT", new UpdateFieldByName());
			fieldNameHttpMethodsHandlersMap.put("OPTIONS", new FieldNameOptions());
			fieldNameHttpMethodsHandlersMap.put("TRACE", new NotImplemented());
			fieldNameHttpMethodsHandlersMap.put("PATCH", new NotImplemented());
			fieldNameHttpMethodsHandlersMap.put("HEAD", new NotImplemented());
		}
	}
	
	/****************************
	/field_index
	****************************/
	
	class FieldIndexHandler implements HttpHandler {
		private Map<String, HttpHandler> fieldIndexHttpMethodsHandlersMap;

		private FieldIndexHandler() {
			fieldIndexHttpMethodsHandlersMap = new HashMap<>();
			initFieldIndexHandlers();
		}
		
		@Override
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		fieldIndexHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				String errJson = createJsonMessage(ERROR_MESSAGE, BAD_REQUEST);
				sendJsonMessage(exchange, HttpStatusCode.BAD_REQUEST, errJson);
			}
	    }
	    
		private class ReadFieldByIndex implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				URI uri = exchange.getRequestURI();
				String databaseName = UriParser.getDbName(uri);
				String tableName = UriParser.getTableName(uri);
				String primaryKeyColumnName = UriParser.getQueryMap(uri).get(PK_NAME);
				Object primaryKeyValue =  UriParser.getQueryMap(uri).get(PK_VALUE);
				Integer columnIndex = Integer.parseInt(UriParser.getQueryMap(uri).get(COLUMN_INDEX));
				Object result = null;			
				try {
					createDatabaseIfNotInMap(databaseName);
					result = companyDatabases.get(databaseName).readField(tableName, primaryKeyColumnName, primaryKeyValue, columnIndex);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}			
				String json = createJsonMessage("fieldValue", (String)result);
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class UpdateFieldByIndex implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				URI uri = exchange.getRequestURI();
				String databaseName = UriParser.getDbName(uri);
				String tableName = UriParser.getTableName(uri);
				JSONObject body = getJsonBody(exchange);
				String primaryKeyColumnName = (String) body.get(PRIMARY_KEY_NAME);
				Object primaryKeyValue = (String) body.get(PRIMARY_KEY_VALUE);
				Integer columnIndex = Integer.parseInt((String)body.get(COLUMN_INDEX));
				Object newValue = body.get(NEW_VALUE);			
				try {
					createDatabaseIfNotInMap(databaseName);
					companyDatabases.get(databaseName).updateField(tableName, primaryKeyColumnName, primaryKeyValue, columnIndex, newValue);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}			
				String json = createJsonMessage(MESSAGE, "field updated");
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class FieldIndexOptions implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				Headers headers = exchange.getResponseHeaders();
				headers.add("Allow", FIELD_INDEX_OPTIONS);
				sendJsonMessage(exchange, HttpStatusCode.OK, null);
			}
		}
		
		private void initFieldIndexHandlers() {
			fieldIndexHttpMethodsHandlersMap.put("GET", new ReadFieldByIndex());
			fieldIndexHttpMethodsHandlersMap.put("PUT", new UpdateFieldByIndex());
			fieldIndexHttpMethodsHandlersMap.put("OPTIONS", new FieldIndexOptions());
			fieldIndexHttpMethodsHandlersMap.put("TRACE", new NotImplemented());
			fieldIndexHttpMethodsHandlersMap.put("PATCH", new NotImplemented());
			fieldIndexHttpMethodsHandlersMap.put("HEAD", new NotImplemented());
		}
	}

	/****************************
	/table
	****************************/
	
	class TableHandler implements HttpHandler {
		private Map<String, HttpHandler> tableHttpMethodsHandlersMap;

		private TableHandler() {
			tableHttpMethodsHandlersMap = new HashMap<>();
			initTableHandlers();
		}
		
		@Override
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		tableHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();			
				String errJson = createJsonMessage(ERROR_MESSAGE, BAD_REQUEST);
				sendJsonMessage(exchange, HttpStatusCode.BAD_REQUEST, errJson);
			}
	    }
	    
		private class CreateTable implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				JSONObject jason = getJsonBody(exchange);
				String sqlCommand = (String) jason.get(SQL_COMMAND);		
				try {
					createDatabaseIfNotInMap(databaseName);
					companyDatabases.get(databaseName).createTable(sqlCommand);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}
				
				String json = createJsonMessage(MESSAGE, "table created");
				sendJsonMessage(exchange, HttpStatusCode.CREATED, json);
			}
		}
		
		private class DeleteTable implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				URI uri = exchange.getRequestURI();
				String databaseName = UriParser.getDbName(uri);
				String tableName = UriParser.getTableName(uri);		
				try {
					createDatabaseIfNotInMap(databaseName);
					companyDatabases.get(databaseName).deleteTable(tableName);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}			
				String json = createJsonMessage(MESSAGE, "table deleted");
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class TableOptions implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				Headers headers = exchange.getResponseHeaders();
				headers.add("Allow", TABLE_OPTIONS);
				sendJsonMessage(exchange, HttpStatusCode.OK, null);
			}
		}
		
		private void initTableHandlers() {
			tableHttpMethodsHandlersMap.put("POST", new CreateTable());
			tableHttpMethodsHandlersMap.put("DELETE", new DeleteTable());
			tableHttpMethodsHandlersMap.put("OPTIONS", new TableOptions());
			tableHttpMethodsHandlersMap.put("TRACE", new NotImplemented());
			tableHttpMethodsHandlersMap.put("PATCH", new NotImplemented());
			tableHttpMethodsHandlersMap.put("HEAD", new NotImplemented());
		}
	}
	
	/****************************
	/iotEvent
	****************************/
	
	class IotEventHandler implements HttpHandler {
		private Map<String, HttpHandler> iotEventHttpMethodsHandlersMap;

		private IotEventHandler() {
			iotEventHttpMethodsHandlersMap = new HashMap<>();
			initIotEventHandlers();
		}
		
		@Override
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		iotEventHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				String errJson = createJsonMessage(ERROR_MESSAGE, BAD_REQUEST);
				sendJsonMessage(exchange, HttpStatusCode.BAD_REQUEST, errJson);
			}
	    }
	    
		private class CreateIotEvent implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				JSONObject jason = getJsonBody(exchange);
				String rawData = (String) jason.get(RAW_DATA);			
				try {
					createDatabaseIfNotInMap(databaseName);
					companyDatabases.get(databaseName).createIOTEvent(rawData);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}			
				String json = createJsonMessage(MESSAGE, "IOT event created");
				sendJsonMessage(exchange, HttpStatusCode.CREATED, json);
			}
		}

		
		private class IotEventOptions implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				Headers headers = exchange.getResponseHeaders();
				headers.add("Allow", IOT_EVENT_OPTIONS);
				sendJsonMessage(exchange, HttpStatusCode.OK, null);
			}
		}
		
		private void initIotEventHandlers() {
			iotEventHttpMethodsHandlersMap.put("POST", new CreateIotEvent());
			iotEventHttpMethodsHandlersMap.put("OPTIONS", new IotEventOptions());
			iotEventHttpMethodsHandlersMap.put("TRACE", new NotImplemented());
			iotEventHttpMethodsHandlersMap.put("PATCH", new NotImplemented());
			iotEventHttpMethodsHandlersMap.put("HEAD", new NotImplemented());
		}
	}
	
	/****************************
	/*********utilities
	****************************/
	
	class NotImplemented implements HttpHandler {
		@Override
	    public void handle(HttpExchange exchange) throws IOException {
			String json = createJsonMessage(ERROR_MESSAGE, "not implemented");
			sendJsonMessage(exchange, HttpStatusCode.NOT_IMPLEMENTED, json);
	    }
	}
	
	class AllOptions implements HttpHandler {
		@Override
	    public void handle(HttpExchange exchange) throws IOException {
			Headers headers = exchange.getResponseHeaders();
			headers.add("Allow", ALL_OPTIONS);
			sendJsonMessage(exchange, HttpStatusCode.OK, null);
	    }
	}
	
	private JSONObject getJsonBody(HttpExchange exchange) throws IOException {
		InputStream is = exchange.getRequestBody();
		String body = getStringFromInputStream(is);
				
		return new JSONObject(body);
	}
	
	private String getStringFromInputStream(InputStream inputStream) throws IOException {
		BufferedInputStream bis = new BufferedInputStream(inputStream);
		ByteArrayOutputStream buf = new ByteArrayOutputStream();
		int result = bis.read();
		while(result != -1) {
		    buf.write((byte) result);
		    result = bis.read();
		}
		return buf.toString("UTF-8");
	}
	
	private void sendJsonMessage(HttpExchange exchange, HttpStatusCode statusCode, String jsonMessage) throws IOException {
		fillResponseHeaders(exchange);
		int jsonLength = 0;
		if(null != jsonMessage) {
			jsonLength = jsonMessage.length();
		}
		exchange.sendResponseHeaders(statusCode.getCode(), jsonLength);
	    OutputStream os = exchange.getResponseBody();
	    if(null != jsonMessage) {
	    	os.write(jsonMessage.getBytes());
	    }
	    os.close();
	}
	
	private void fillResponseHeaders(HttpExchange exchange) {
		Headers headers = exchange.getResponseHeaders();
		headers.add("Connection", "close");
		headers.add("Connection-Type", "application/json");
	}
			
	private String createJsonMessage(String message, List<Object> paramList) {
		if(1 == paramList.size()) {
			return "{" + "\"" + message + "\"" +  ":" + "\"" + paramList.get(0) + "\"" + "}";
		}
		return "{" + "\"" + message + "\"" +  ":" + "\"" + paramList.toString() + "\"" + "}";
	}
	
	private String createJsonMessage(String leftMessage, String rightMessage) {
		return "{" + "\"" + leftMessage + "\"" +  ":" + "\"" + rightMessage + "\"" + "}";
	}
	
	private void createDatabaseIfNotInMap(String databaseName) throws SQLException {
		if(null == companyDatabases.get(databaseName)) {
			DatabaseManagement dbManager = null;
			dbManager = new DatabaseManagement(URL, USER_NAME, PASSWORD, databaseName);
			companyDatabases.put(databaseName, dbManager);
		}
	}
}