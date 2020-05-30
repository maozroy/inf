package il.co.ilrd.databasemanagementserver;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import il.co.ilrd.chatserver.GlobalChatProtocolKeys;
import il.co.ilrd.chatserver.GlobalProtocolPort;
import il.co.ilrd.chatserver.GlobalProtocolType;
import il.co.ilrd.chatserver.GlobalServerMessage;
import il.co.ilrd.chatserver.ObjectCovertUtil;



public class testing {
	static ServerMessage msg = new ServerMessage(
			ProtocolType.DATABASE_MANAGEMENT, 
			new DatabaseManagementMessage(
					new ActionTypeKey(
							"tadi", 
							DatabaseKeys.CREATE_COMPANY_DATABASE),
					null));
	
	
	public static void main(String[] args) throws SQLException, ClassNotFoundException, IOException{
		String hostName = InetAddress.getLocalHost().getHostName();
		int portNumber = ProtocolPort.DATABASE_MANAGEMENT_PORT.getPort();
		ByteBuffer buffer = ByteBuffer.allocate(4096);
		SocketChannel clientSocket = null;


		ServerMessage receivedMssage = null;
		DatabaseManagementMessage dbMsg = null;
		byte[] messageArray =  ObjectCovertUtil.toByteArray(msg);
		try {
			clientSocket = SocketChannel.open(new InetSocketAddress(hostName, portNumber));
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		// send register message
		messageArray =  ObjectCovertUtil.toByteArray(msg);
		buffer.clear();
		buffer.put(messageArray);
		buffer.flip();
		clientSocket.write(buffer);
		
		buffer.clear();
		clientSocket.read(buffer);
		receivedMssage = (ServerMessage) ConversionUtils.toObject(buffer.array()); 
		dbMsg = (DatabaseManagementMessage) receivedMssage.getData();
		PrintResponse(receivedMssage);
		LinkedList<Object> list = new LinkedList<Object>();
		list.add(0, "Students");
		list.add(1, "Name");
		list.add(2, "'Maoz'");
	//	list.add(3, "Age");

		msg = new ServerMessage(
				ProtocolType.DATABASE_MANAGEMENT, 
				new DatabaseManagementMessage(
						new ActionTypeKey(
								"Infinity", 
								DatabaseKeys.READ_FIELD_BY_NAME),
						list));
		messageArray =  ObjectCovertUtil.toByteArray(msg);
		buffer.clear();
		buffer.put(messageArray);
		buffer.flip();
		clientSocket.write(buffer);
		
		buffer.clear();
		clientSocket.read(buffer);
		receivedMssage = (ServerMessage) ConversionUtils.toObject(buffer.array()); 
		dbMsg = (DatabaseManagementMessage) receivedMssage.getData();
		PrintResponse(receivedMssage);
}

	private static void PrintResponse(ServerMessage receivedMssage) {
		DatabaseManagementMessage dbMsg = (DatabaseManagementMessage) receivedMssage.getData();
		List<Object> listReturned = dbMsg.getData();
		System.out.println("key: " + dbMsg.getKey().getActionType());
		for (Object object : listReturned) {
			System.out.println(object);
		}
	}
}
