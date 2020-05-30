package il.co.ilrd.httpiotserver;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.sql.SQLException;
import java.util.LinkedList;

import il.co.ilrd.chatserver.ObjectCovertUtil;
import il.co.ilrd.databasemanagementserver.ActionTypeKey;
import il.co.ilrd.databasemanagementserver.ConversionUtils;
import il.co.ilrd.databasemanagementserver.DatabaseKeys;
import il.co.ilrd.databasemanagementserver.DatabaseManagementMessage;
import il.co.ilrd.databasemanagementserver.ProtocolPort;
import il.co.ilrd.databasemanagementserver.ProtocolType;
import il.co.ilrd.databasemanagementserver.ServerMessage;

public class ClientTest {
	static String HTTPMsg = "GET Tadiran/country?method=READ_ROW&pkName=country_name&pkValue=Israel1 HTTP/1.1\r\n" + 
			"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n" + 
			"Host: http://localhost/ \r\n" + 
			"Content-Type: application/x-www-form-urlencoded\r\n" + 
			"Content-Length: length\r\n" + 
			"Accept-Language: en-us\r\n" + 
			"Accept-Encoding: gzip, deflate\r\n" + 
			"Connection: Keep-Alive\r\n" + 
			"\r\n" + 
			"licenseID=string&content=string&/paramsXML=string";
	
	public static void main(String[] args) throws SQLException, ClassNotFoundException, IOException{

		String hostName = InetAddress.getLocalHost().getHostName();
		int portNumber = 8080;
		ByteBuffer buffer = ByteBuffer.allocate(4096);
		SocketChannel clientSocket = null;

		ServerMessage receivedMssage = null;
		byte[] messageArray =  ConversionUtils.toByteArray(HTTPMsg);
		
		try {
			clientSocket = SocketChannel.open(new InetSocketAddress(hostName, portNumber));
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		// send register message
		messageArray =  ConversionUtils.toByteArray(HTTPMsg);
		buffer.clear();
		buffer.put(messageArray);
		buffer.flip();
		clientSocket.write(buffer);
		
//		buffer.clear();
//		clientSocket.read(buffer);
//		receivedMssage = (ServerMessage) ConversionUtils.toObject(buffer.array()); 
//		dbMsg = (DatabaseManagementMessage) receivedMssage.getData();
//		PrintResponse(receivedMssage);
//		LinkedList<Object> list = new LinkedList<Object>();
//		list.add(0, "Students");
//		list.add(1, "Name");
//		list.add(2, "'Maoz'");
//	//	list.add(3, "Age");
//
//		msg = new ServerMessage(
//				ProtocolType.DATABASE_MANAGEMENT, 
//				new DatabaseManagementMessage(
//						new ActionTypeKey(
//								"Infinity", 
//								DatabaseKeys.READ_FIELD_BY_NAME),
//						list));
//		messageArray =  ObjectCovertUtil.toByteArray(msg);
//		buffer.clear();
//		buffer.put(messageArray);
//		buffer.flip();
//		clientSocket.write(buffer);
//		
//		buffer.clear();
//		clientSocket.read(buffer);
//		receivedMssage = (ServerMessage) ConversionUtils.toObject(buffer.array()); 
//		dbMsg = (DatabaseManagementMessage) receivedMssage.getData();
//		PrintResponse(receivedMssage);
}
}
