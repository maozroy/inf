package il.co.ilrd.server.chatserver;


import java.io.Serializable;

public class ChatServerMessage implements Message<ChatProtocolKeys, String> , Serializable{
	private static final long serialVersionUID = 1L;
	private ChatProtocolKeys key;
	private String data;
	
	public ChatServerMessage (ChatProtocolKeys key, String data) {
		this.key = key;
		this.data = data;
	}
	
	@Override
	public ChatProtocolKeys getKey() {
		return key;
	}
	
	@Override
	public String getData() {
		return data;
	}
	
	public void setMessage(String newMessage) {
		data = newMessage;
	}
	
	@Override
	public String toString() {
		return data;
	}	
}
