package il.co.ilrd.DatabaseManagement;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import il.co.ilrd.gatewayserver.GatewayServer.DatabaseManagementInterface;

public class DatabaseManagement implements DatabaseManagementInterface {
	private final String databaseName;
	private Connection connection;
	private Statement statement;
	private final String url;
	private final String userName;
	private final String password;
		
	public DatabaseManagement(String url, String userName, String password, String databaseName) throws SQLException {
		Objects.requireNonNull(url);
		Objects.requireNonNull(userName);
		Objects.requireNonNull(password);
		Objects.requireNonNull(databaseName);
		
		this.databaseName = Objects.requireNonNull(databaseName);
		this.url = Objects.requireNonNull(url);
		this.userName = Objects.requireNonNull(userName);
		this.password = Objects.requireNonNull(password);
		createDatabase();
	}
	
	public void createTable(String sqlCommand) throws SQLException {
		executeSqlCommand(sqlCommand);
	}
	
	public void deleteTable(String tableName) throws SQLException {
		executeSqlCommand("DROP TABLE " + tableName);
	}
	
	public void createRow(String sqlCommand) throws SQLException {
		Objects.requireNonNull(sqlCommand);
		
		executeSqlCommand(sqlCommand);
	}
	
	/* rawData example: "serialNumber|description|timeStamp", user is responsible for quotes */
	public void createIOTEvent(String rawData) throws SQLException {
		Objects.requireNonNull(rawData);
		
		final String DELIMITER = "\\|";
		String[] tokens = rawData.split(DELIMITER);
		String query = "INSERT into IOTEvent values(null ";
		for (int i = 0; i < tokens.length; ++i) {
			tokens[i] = "," + tokens[i];
			query += tokens[i];
		}
		query += ")";		
		createRow(query);
	}
	
	public List<Object> readRow(String tableName, String primaryKeyColumnName, Object primaryKeyValue) throws SQLException {
		Objects.requireNonNull(tableName);
		Objects.requireNonNull(primaryKeyColumnName);
		Objects.requireNonNull(primaryKeyValue);
			
        List<Object> rowAsList;
		try {
			connectToSQL();
			ResultSet result = readRowImpl(tableName, primaryKeyValue, primaryKeyColumnName);
	    	result.next();
	    	rowAsList = new ArrayList<>();
	    	
	        for(int i = 1; i <= result.getMetaData().getColumnCount(); ++i) {
	        	rowAsList.add(result.getObject(i));
	        }
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();

		return rowAsList;
	}
	
	public Object readField(String tableName, String primaryKeyColumnName, Object primaryKey, int columnIndex) throws SQLException {
		Objects.requireNonNull(tableName);
		Objects.requireNonNull(primaryKeyColumnName);
		Objects.requireNonNull(primaryKey);
		
		Object res;
		try {
			connectToSQL();
			ResultSet result = readRowImpl(tableName, primaryKey, primaryKeyColumnName);
			result.next();
			res = result.getObject(columnIndex);
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();

		return res;
	}
	
	public Object readField(String tableName, String primaryKeyColumnName, Object primaryKey, String columnName) throws SQLException {
		Objects.requireNonNull(tableName);
		Objects.requireNonNull(primaryKeyColumnName);
		Objects.requireNonNull(primaryKey);
		Objects.requireNonNull(columnName);
		
		Object res;
		try {
			connectToSQL();
			ResultSet result = readRowImpl(tableName, primaryKey, primaryKeyColumnName);
			result.next();
			res = result.getObject(columnName);
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();
		
		return res;
	}
	
	public void updateField(String tableName, String primaryKeyColumnName, Object primaryKey, int columnIndex, Object newValue) throws SQLException {
		Objects.requireNonNull(tableName);
		Objects.requireNonNull(primaryKeyColumnName);
		Objects.requireNonNull(primaryKey);
		Objects.requireNonNull(newValue);		
		
		try {
			connectToSQL();
			ResultSet result = readRowImpl(tableName, primaryKey, primaryKeyColumnName);
			result.next();
			result.updateObject(columnIndex, newValue);
			result.updateRow();
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();
	}
	
	public void updateField(String tableName, String primaryKeyColumnName, Object primaryKey, String columnName, Object newValue) throws SQLException {
		Objects.requireNonNull(tableName);
		Objects.requireNonNull(primaryKeyColumnName);
		Objects.requireNonNull(primaryKey);
		Objects.requireNonNull(columnName);
		Objects.requireNonNull(newValue);	
		
		final String query = "UPDATE " + tableName + " SET " + columnName + "="  +  newValue  +  " WHERE " + primaryKeyColumnName + "=" + primaryKey;
		executeSqlCommand(query);
	}
	
	public void deleteRow(String tableName, String primaryKeyColumnName, Object primaryKey) throws SQLException {
		Objects.requireNonNull(tableName);
		Objects.requireNonNull(primaryKeyColumnName);
		Objects.requireNonNull(primaryKey);

		final String query = "DELETE FROM " + tableName + " WHERE " + primaryKeyColumnName + "=" + primaryKey;
		executeSqlCommand(query);
	}
	
	private void createDatabase() throws SQLException {
		try(
				Connection connection = DriverManager.getConnection(url, userName, password);
				Statement statement = connection.createStatement();
		){
			statement.execute("CREATE DATABASE IF NOT EXISTS " + databaseName);
		}
	}
	
	private void connectToSQL() throws SQLException {
		connection = DriverManager.getConnection(url + databaseName, userName, password);
		statement = connection.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);
	}
	
	public void executeSqlCommand(String sqlCommands) throws SQLException {
		Objects.requireNonNull(sqlCommands);
		connectToSQL();
		final String COMMAND_DELIMETER = ";";
		String[] sqlCommandsArr = sqlCommands.split(COMMAND_DELIMETER);
		connection.setAutoCommit(false);	
		for (String command : sqlCommandsArr) {
			System.out.println("commnad that will be executed: \n" + command.trim());
			statement.addBatch(command.trim());			
		}
		statement.executeBatch();
		connection.commit();
	}
	
//	private void executeSqlCommand(String sqlCommand) throws SQLException {
//		Objects.requireNonNull(sqlCommand);
//				
//		try {
//			connectToSQL();
//			statement.executeUpdate(sqlCommand);
//		} catch (SQLException e) {
//			closeResource();
//			throw e;
//		}
//		closeResource();
//	}
	
	private void closeResource() throws SQLException {
		if(null != statement) {
			statement.close();
			statement = null;
		}
		
		if(null != statement) {
			connection.close();
			connection = null;
		}
	}
	
	//Requires connection be initialized before call
	private ResultSet readRowImpl(String tableName, Object rowKey, String keyColumnName) throws SQLException {
		final String query = "SELECT * FROM " + tableName + " WHERE " + keyColumnName + " = "  + rowKey;
		ResultSet result = statement.executeQuery(query);
		
		return result;
	}
}