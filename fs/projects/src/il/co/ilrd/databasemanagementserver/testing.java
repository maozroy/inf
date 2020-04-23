package il.co.ilrd.databasemanagementserver;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;



public class testing {

	public static void main(String[] args) throws SQLException, ClassNotFoundException{
		DatabaseManagement dbmng = new DatabaseManagement("tadiran", "jdbc:mysql://localhost/", "root", "root");
//		dbmng.createRow("insert into Product(product_name) "
//				+ "VALUES ('BestAC')");
//		dbmng.createRow("insert into IOTItem "
//				+ "VALUES ('0001', 1)");

		dbmng.createIOTEvent("0001 | Fire in the hole " );
//		dbmng.deleteTable("Country");
//		dbmng.createTable("CREATE TABLE Country(\n" + 
//				"CountryID INT AUTO_INCREMENT PRIMARY KEY,\n" + 
//				"\n" + 
//				"CountryName VARCHAR(20) UNIQUE\n" + 
//				"\n" + 
//				");");
//		dbmng.createTable(""
//		dbmng.createRow("insert into Country(CountryName) "
//				+ "VALUES ('Israel')");
//		dbmng.createRow("insert into Country(CountryName) "
//				+ "VALUES ('USA')");
//		dbmng.createRow("insert into Country(CountryName) "
//				+ "VALUES ('Germany')");
//		dbmng.createRow("insert into Country(CountryName) "
//				+ "VALUES ('LalaLAnd')");
//		dbmng.updateField("Country", "CountryName", "LalaLAnd", 2, "LalaLand");
//		System.out.println(dbmng.readField("Country", "CountryID", 4, 2));
//		List<Object> list = dbmng.readRow("Country", "CountryID", "1");
//		for (int i = 0; i < list.size(); i++) {
//			System.out.println(list.get(i).toString());
//		}
		
	//	dbmng.deleteRow("Country", "CountryID", 2);
	

}
}
