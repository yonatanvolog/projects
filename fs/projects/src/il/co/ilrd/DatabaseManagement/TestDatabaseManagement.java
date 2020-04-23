package il.co.ilrd.DatabaseManagement;

import java.io.IOException;
import java.sql.SQLException;
import java.util.List;

public class TestDatabaseManagement {
	static int testNumber = 1;
	
	public static void main(String[] args) throws SQLException {
		String dbName = "tadiran";
		String url = "jdbc:mysql://localhost:3306/";
		String uname = "root";
		String pass = "";
		DatabaseManagement manager = new DatabaseManagement(dbName, url, uname, pass);

		WaitForKeyPress();
		test1_createTable(manager);
		
		WaitForKeyPress();
		test2_createRow(manager);
		
		WaitForKeyPress();
		test3_createTable(manager);
		
		WaitForKeyPress();
		test4_createRow(manager);
		
		WaitForKeyPress();
		test5_deleteRow(manager);

		WaitForKeyPress();
		test6_deleteTable(manager);
		
		WaitForKeyPress();
		test7_updateField1(manager);

		WaitForKeyPress();
		test8_updateField2(manager);
		
		WaitForKeyPress();
		test9_readRow(manager);
		
		WaitForKeyPress();
		test10_readField1(manager);
		
		WaitForKeyPress();
		test11_readField2(manager);
		
		WaitForKeyPress();
		test12_createIOTEvent(manager);
		
		te1(manager);
		te2(manager);
	}
	
	private static void test1_createTable(DatabaseManagement manager) throws SQLException {
		manager.createTable("CREATE TABLE Parent (\n" + 
				"	parentID int AUTO_INCREMENT PRIMARY KEY,\n" + 
				"    name varchar(40) NOT NULL,\n" + 
				"    job varchar(20) NOT NULL,\n" + 
				"    address varchar(20) NOT NULL\n" + 
				");");
	}
	
	private static void test2_createRow(DatabaseManagement manager) throws SQLException {
		manager.createRow("INSERT into Parent values(null, \"Yossi\", \"Jr Dev\",\"Tel Aviv, Yam 1\");");
	}
	
	private static void test3_createTable(DatabaseManagement manager) throws SQLException {
		manager.createTable("CREATE TABLE Child (\n" + 
				"    childID int AUTO_INCREMENT PRIMARY KEY,\n" + 
				"    name varchar(40) NOT NULL,\n" + 
				"    FK_parentID int NOT NULL,\n" + 
				"    class int NOT NULL\n" + 
				");");
	}
	
	private static void test4_createRow(DatabaseManagement manager) throws SQLException {
		manager.createRow("INSERT into Child values(null, \"Yossi jr\", 1, 6);");
	}
	
	private static void test5_deleteRow(DatabaseManagement manager) throws SQLException {
		manager.deleteRow("Child", "childID", "1");
	}
	
	private static void test6_deleteTable(DatabaseManagement manager) throws SQLException {
		manager.deleteTable("Child");
	}
	
	private static void test7_updateField1(DatabaseManagement manager) throws SQLException {
		manager.updateField("Parent", "parentID", "1", "job", "Engineer");
	}
	
	private static void test8_updateField2(DatabaseManagement manager) throws SQLException {
		int jobColumnIndex = 3;
		manager.updateField("Parent", "parentID", "1", jobColumnIndex, "Tech Lead!");
	}
	
	private static void test9_readRow(DatabaseManagement manager) throws SQLException {
		List<Object> row = manager.readRow("Parent", "parentID", "1");
		System.out.println("Listing elements in Parent, where parentID=1");
		for (Object object : row) {
			System.out.println(object.toString());
		}
	}
	
	private static void test10_readField1(DatabaseManagement manager) throws SQLException {
		Object field = manager.readField("Parent","parentID", "1", "job");
		System.out.println(field.toString());
	}
	
	private static void test11_readField2(DatabaseManagement manager) throws SQLException {
		int jobColumnIndex = 3;
		Object field = manager.readField("Parent", "parentID", "1", jobColumnIndex);
		System.out.println(field.toString());
	}
	
	private static void test12_createIOTEvent(DatabaseManagement manager) throws SQLException {
		manager.createIOTEvent("00001|software update 1.2|null");
		
		List<Object> row = manager.readRow("IOTEvent", "iot_event_id", "2");
		System.out.println("Listing elements in IOTEvent, where iot_event_id=2");
		for (Object object : row) {
			System.out.println(object.toString());
		}
	}

	private static void WaitForKeyPress() {
		System.out.println("Will run test number "  + testNumber + ", press key");
		++testNumber;
		try {
			System.in.read();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private static void te1(DatabaseManagement manager) throws SQLException {
		manager.createRow("INSERT into Parent values(null, \"Avi\", \"Manger\",\"Jerusalem\");");
		manager.createRow("INSERT into Parent values(null, \"Moti\", \"Technician\",\"Beer Sehva\");");
		manager.createRow("INSERT into Parent values(null, \"Nachum\", \"Mentor\",\"Ramat Gan\");");
	}
	
	private static void te2(DatabaseManagement manager) throws SQLException {
		int jobColumnIndex = 3;
		Object field;
		System.out.println("Avi 2 versions");
		field = manager.readField("Parent","parentID", "2", "job");
		System.out.println(field);
		
		field = manager.readField("Parent", "parentID", "2", jobColumnIndex);
		System.out.println(field);

		
		System.out.println("Moti 2 versions");
		field = manager.readField("Parent","parentID", "3", "job");
		System.out.println(field);
		
		field = manager.readField("Parent", "parentID", "3", jobColumnIndex);
		System.out.println(field);

		
		System.out.println("Nachum 2 versions");
		field = manager.readField("Parent","parentID", "4", "job");
		System.out.println(field);
		
		field = manager.readField("Parent", "parentID", "4", jobColumnIndex);
		System.out.println(field);
	}
}
