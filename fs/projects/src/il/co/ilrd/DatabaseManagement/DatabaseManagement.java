package il.co.ilrd.DatabaseManagement;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import com.mysql.cj.Query;

public class DatabaseManagement {
	private final String databaseName;
	private Connection connection;
	private Statement statement;
	private final String url;
	private final String userName;
	private final String password;
		
	public DatabaseManagement(String databaseName, String url, String userName, String password) {
		this.databaseName = Objects.requireNonNull(databaseName);
		this.url = Objects.requireNonNull(url);
		this.userName = Objects.requireNonNull(userName);
		this.password = Objects.requireNonNull(password);
	}
	
	public void createTable(String sqlCommand) throws SQLException {
		Objects.requireNonNull(sqlCommand);
		
		try {
			connectToSQL();
			statement.executeUpdate(sqlCommand);
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();
	}
	
	public void deleteTable(String tableName) throws SQLException {
		Objects.requireNonNull(tableName);
		
		try {
			connectToSQL();
			final String query = "DROP TABLE " + tableName;
			statement.executeUpdate(query);
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();
	}
	
	public void createRow(String sqlCommand) throws SQLException {
		Objects.requireNonNull(sqlCommand);
		
		try {
			connectToSQL();
			statement.executeUpdate(sqlCommand);
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();
	}
	
	/* rawData example: "serialNumber|description|timeStamp", timeStamp is optional */
	public void createIOTEvent(String rawData) throws SQLException {
		Objects.requireNonNull(rawData);
		
		final String DELIMITER = "\\|";
		String[] tokens = rawData.split(DELIMITER);
		String query = "INSERT into IOTEvent values(null ";
		for (int i = 0; i < tokens.length; ++i) {
			if(0 == i | 1 == i) {
				tokens[i] = "\"" + tokens[i] + "\"";
			}
			tokens[i] = "," + tokens[i];
			query += tokens[i];
		}
		query += ");";
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
		
		try {
			connectToSQL();
			final String query = " UPDATE " + tableName + " SET " + columnName + "="  + "'" +  newValue  + "'" +  " WHERE " + primaryKeyColumnName + "=" + primaryKey +";";
			statement.executeUpdate(query);
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();
	}
	
	public void deleteRow(String tableName, String primaryKeyColumnName, Object primaryKey) throws SQLException {
		Objects.requireNonNull(tableName);
		Objects.requireNonNull(primaryKeyColumnName);
		Objects.requireNonNull(primaryKey);
		
		try {
			connectToSQL();
			final String query = "DELETE FROM " + tableName + " WHERE " + primaryKeyColumnName + "=" + primaryKey + ";";
			statement.executeUpdate(query);
		} catch (SQLException e) {
			closeResource();
			throw e;
		}
		closeResource();
	}
	
	private void connectToSQL() throws SQLException {
		connection = DriverManager.getConnection(url + databaseName, userName, password);
		statement = connection.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);
	}
	
	private void closeResource() throws SQLException {
		statement.close();
		connection.close();
	}
	
	//Requires connection be initialized before call
	private ResultSet readRowImpl(String tableName, Object rowKey, String keyColumnName) throws SQLException {
		final String query = "SELECT * FROM " + tableName + " WHERE " + keyColumnName + " = "  + rowKey;
		ResultSet result = statement.executeQuery(query);
		
		return result;
	}
}