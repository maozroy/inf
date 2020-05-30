package il.co.ilrd.httpiotserver;

import java.sql.DriverManager;
import java.sql.SQLException;


public class ServerTest {

	public static void main(String[] args) throws SQLException {

	//	java.sql.Connection connection = DriverManager.getConnection("jdbc:mysql://127.0.0.1/", "root", "root");

		HttpServer server = new HttpServer();
		server.addHttpConnection(8080);
		server.run();
	}
}
