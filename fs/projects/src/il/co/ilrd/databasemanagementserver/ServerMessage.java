package il.co.ilrd.databasemanagementserver;

import java.io.Serializable;

import il.co.ilrd.chatserver.GlobalMessage;


public class ServerMessage implements GlobalMessage<ProtocolType, GlobalMessage<?, ?>> ,Serializable{
		
	private static final long serialVersionUID = 55555;
	private ProtocolType key; 
	private GlobalMessage<?, ?> data;
		
	public ServerMessage(ProtocolType key, GlobalMessage<?, ?> data) {
		this.key = key;
		this.data = data;
	}

	@Override
	public ProtocolType getKey() {
		return key;
	}


	public void setData(GlobalMessage<?, ?> data) {
		this.data = data;
	}

	@Override
	public GlobalMessage<?, ?> getData() {
		// TODO Auto-generated method stub
		return data;
	}
}