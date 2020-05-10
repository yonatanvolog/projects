package il.co.ilrd.sunhttpserver;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONObject;

import com.sun.net.httpserver.Headers;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import il.co.ilrd.DatabaseManagement.DatabaseManagement;
import il.co.ilrd.http_message.HttpStatusCode;
import il.co.ilrd.httpiotserver.DatabaseKeys;

class BadRequestException extends RuntimeException {}

public class SunHttpServer {
	
	private static final String URL = "jdbc:mysql://localhost:3306/";
	private static final String USER_NAME = "root";
	private static final String PASSWORD = "";
	
	static final String PK_NAME = "pkName";
	static final String PK_VALUE = "pkValue";
	static final String COLUMN_NAME = "columnName";
	static final String COLUMN_INDEX = "columnIndex";
	static final String SQL_COMMAND = "sqlCommand";
	static final String RAW_DATA = "rawData";
	static final String PRIMARY_KEY_NAME = "primaryKeyName";
	static final String PRIMARY_KEY_VALUE = "primaryKeyValue";
	static final String NEW_VALUE = "newValue";
	static final String TABLE_NAME = "tableName";

	
	HttpServer javaHttpServer;

	private Map<String, DatabaseManagement> companyDatabases = new HashMap<>();

	
	Map<String, HttpHandler> rowHttpMethodsHandlersMap = new HashMap<>();
	Map<String, HttpHandler> fieldNameHttpMethodsHandlersMap = new HashMap<>();
	Map<String, HttpHandler> fieldIndexHttpMethodsHandlersMap = new HashMap<>();
	Map<String, HttpHandler> tableHttpMethodsHandlersMap = new HashMap<>();
	Map<String, HttpHandler> iotEventHttpMethodsHandlersMap = new HashMap<>();

	public SunHttpServer(int portNumber) throws IOException {
		javaHttpServer = HttpServer.create(new InetSocketAddress(portNumber), 0);
		javaHttpServer.setExecutor(null);

		javaHttpServer.createContext("/row", new RowHandler());
		javaHttpServer.createContext("/field_name", new FieldNameHandler());
		javaHttpServer.createContext("/field_index", new FieldIndexHandler());
		javaHttpServer.createContext("/table", new TableHandler());
		javaHttpServer.createContext("/iotEvent", new IotEventHandler());
	}
	
	public void start() {
		javaHttpServer.start();
	}
	
	class RowHandler implements HttpHandler {
		private RowHandler() {
	    	initRowHandlers();
		}
		
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		rowHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				System.err.println("Bad request! 1");
			}
	    }
	    
		private class ReadRow implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				System.out.println("int ReadRow");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
				System.out.println("table: " + UriParser.getTableName(exchange.getRequestURI()));
				System.out.println("PkName: " + UriParser.getQueryMap(exchange.getRequestURI()).get(PK_NAME));
				System.out.println("PkValue: " + UriParser.getQueryMap(exchange.getRequestURI()).get(PK_VALUE));
				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				String tableName = UriParser.getTableName(exchange.getRequestURI());
				String primaryKeyColumnName = UriParser.getQueryMap(exchange.getRequestURI()).get(PK_NAME);
				Object primaryKeyValue =  UriParser.getQueryMap(exchange.getRequestURI()).get(PK_VALUE);

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
				System.out.println("in CreateRow");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
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
				
				String json = createJsonMessage("message", "row created");
				sendJsonMessage(exchange, HttpStatusCode.CREATED, json);
			}
			
		}
		
		private class DeleteRow implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				System.out.println("in DeleteRow");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				String tableName = UriParser.getTableName(exchange.getRequestURI());
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
				
				String json = createJsonMessage("message", "row deleted");
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class RowOptions implements HttpHandler {//DOESNT WORK YET
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				try {
				System.out.println("in RowOptions");
				List<String> options = new ArrayList<>();
				options.add("GET");
				options.add("POST");
				options.add("DELETE");
				options.add("OPTIONS");
				fillOptionsResponseHeaders(exchange, options);
				sendJsonMessage(exchange, HttpStatusCode.OK, null);
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
				}
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
	
	
	
	class FieldNameHandler implements HttpHandler {
		private FieldNameHandler() {
			initFieldNameHandlers();
		}
		
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		fieldNameHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				System.err.println("Bad request! 1");
			}
	    }
	    
		private class ReadFieldByName implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				System.out.println("int ReadFieldByName");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
				System.out.println("table: " + UriParser.getTableName(exchange.getRequestURI()));
				System.out.println("PkName: " + UriParser.getQueryMap(exchange.getRequestURI()).get(PK_NAME));
				System.out.println("PkValue: " + UriParser.getQueryMap(exchange.getRequestURI()).get(PK_VALUE));
				System.out.println("columnName: " + UriParser.getQueryMap(exchange.getRequestURI()).get(COLUMN_NAME));

				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				String tableName = UriParser.getTableName(exchange.getRequestURI());
				String primaryKeyColumnName = UriParser.getQueryMap(exchange.getRequestURI()).get(PK_NAME);
				Object primaryKeyValue =  UriParser.getQueryMap(exchange.getRequestURI()).get(PK_VALUE);
				String columnName = UriParser.getQueryMap(exchange.getRequestURI()).get(COLUMN_NAME);

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
				System.out.println("int UpdateFieldByName");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
				System.out.println("table: " + UriParser.getTableName(exchange.getRequestURI()));
				


				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				String tableName = UriParser.getTableName(exchange.getRequestURI());
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
				
				String json = createJsonMessage("message", "field updated");
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class FieldNameOptions implements HttpHandler {//DOESNT WORK YET
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				
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
	
	class FieldIndexHandler implements HttpHandler {
		private FieldIndexHandler() {
			initFieldIndexHandlers();
		}
		
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		fieldIndexHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				System.err.println("Bad request! 1");
			}
	    }
	    
		private class ReadFieldByIndex implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				System.out.println("int ReadFieldByName");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
				System.out.println("table: " + UriParser.getTableName(exchange.getRequestURI()));

				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				String tableName = UriParser.getTableName(exchange.getRequestURI());
				String primaryKeyColumnName = UriParser.getQueryMap(exchange.getRequestURI()).get(PK_NAME);
				Object primaryKeyValue =  UriParser.getQueryMap(exchange.getRequestURI()).get(PK_VALUE);
				Integer columnIndex = Integer.parseInt(UriParser.getQueryMap(exchange.getRequestURI()).get(COLUMN_INDEX));

				
				


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
				System.out.println("int UpdateFieldByName");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
				System.out.println("table: " + UriParser.getTableName(exchange.getRequestURI()));
				


				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				String tableName = UriParser.getTableName(exchange.getRequestURI());
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
				
				String json = createJsonMessage("message", "field updated");
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class FieldIndexOptions implements HttpHandler {//DOESNT WORK YET
			@Override
			public void handle(HttpExchange exchange) throws IOException {
				
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
	
	
	
	
	
	
	class TableHandler implements HttpHandler {
		private TableHandler() {
			initTableHandlers();
		}
		
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		tableHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				System.err.println("Bad request! 1");
			}
	    }
	    
		private class CreateTable implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				System.out.println("in CreateTable");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
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
				
				String json = createJsonMessage("message", "table created");
				sendJsonMessage(exchange, HttpStatusCode.CREATED, json);
			}
		}
		
		private class DeleteTable implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				System.out.println("in DeleteTable");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
				String databaseName = UriParser.getDbName(exchange.getRequestURI());
				String tableName = UriParser.getTableName(exchange.getRequestURI());
				JSONObject jason = getJsonBody(exchange);
				String bodyTableName = (String) jason.get(TABLE_NAME);
							
				if(!tableName.equals(bodyTableName)) {
					throw new BadRequestException();
				}
				
				try {
					createDatabaseIfNotInMap(databaseName);
					companyDatabases.get(databaseName).deleteTable(tableName);
				} catch (SQLException e) {
					e.printStackTrace();
					throw new BadRequestException();
				}
				
				String json = createJsonMessage("message", "table deleted");
				sendJsonMessage(exchange, HttpStatusCode.OK, json);
			}
		}
		
		private class TableOptions implements HttpHandler {//DOESNT WORK YET
			@Override
			public void handle(HttpExchange exchange) throws IOException {
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
	
	
	
	class IotEventHandler implements HttpHandler {
		private IotEventHandler() {
			initIotEventHandlers();
		}
		
	    public void handle(HttpExchange exchange) throws IOException {
	    	try {
	    		iotEventHttpMethodsHandlersMap.get(exchange.getRequestMethod()).handle(exchange);
	    	} catch (RuntimeException e) {
	    		e.printStackTrace();
				System.err.println("Bad request! 1");
			}
	    }
	    
		private class CreateIotEvent implements HttpHandler {
			@Override
			public void handle(HttpExchange exchange) throws IOException, BadRequestException{
				System.out.println("in CreateIotEvent");
				System.out.println(exchange.getRequestURI());
				System.out.println("dbname: " + UriParser.getDbName(exchange.getRequestURI()));
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
				
				String json = createJsonMessage("message", "IOT event created");
				sendJsonMessage(exchange, HttpStatusCode.CREATED, json);
			}
		}

		
		private class IotEventOptions implements HttpHandler {//DOESNT WORK YET
			@Override
			public void handle(HttpExchange exchange) throws IOException {
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
	
	
	
	
	
	class NotImplemented implements HttpHandler {
	    public void handle(HttpExchange exchange) throws IOException {
			String json = createJsonMessage("errorMessage", "not implemented");
			sendJsonMessage(exchange, HttpStatusCode.NOT_IMPLEMENTED, json);
	    }
	}
	
	
	
	private JSONObject getJsonBody(HttpExchange exchange) throws IOException {
		InputStream is = exchange.getRequestBody();
		String body = getStringFromInputStream(is);
		
		System.out.println("json body recieved:\n" + body);
		
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
	    os.write(jsonMessage.getBytes());
	    os.write("".getBytes());
	    os.close();
	}
	
	private void fillResponseHeaders(HttpExchange exchange) {
		Headers headers = exchange.getRequestHeaders();
		List<String> list = new ArrayList<>();
		list.add("application/json");
		headers.put("Connection-Type", list);
	}
	
	private void fillOptionsResponseHeaders(HttpExchange exchange, List<String> options) {
		Headers headers = exchange.getRequestHeaders();
		List<String> list = new ArrayList<>();
		list.add("application/json");
		headers.put("Connection-Type", list);
		headers.put("Allow", options);
	}
	
	
	private void sendJsonMessage(HttpExchange exchange, String message, List<Object> paramList) {
		
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
	
	private Map<String, String> createHeader(int bodyLength) {
		Map<String, String> header = new HashMap<>();
		header.put("Connection", "close");
		header.put("Connection-Type", "application/json");
		header.put("Content-Length", String.valueOf(bodyLength));
		
		return header;
	}
	
	private Map<String, String> createOptionsHeader(int bodyLength) {
		Map<String, String> header = new HashMap<>();
		header.put("Connection", "close");
		header.put("Connection-Type", "application/json");
		header.put("Allow", "[GET, POST, PUT, DELETE, OPTIONS]");
		
		return header;
	}

	
	private void createDatabaseIfNotInMap(String databaseName) throws SQLException {
		if(null == companyDatabases.get(databaseName)) {
			DatabaseManagement dbManager = null;
			dbManager = new DatabaseManagement(URL, USER_NAME, PASSWORD, databaseName);
			companyDatabases.put(databaseName, dbManager);
		}
	}
}
















