package il.co.ilrd.pingpong.handlers;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class PingPongMessage implements Message<Integer, Message<String, Void>>, Serializable {
	private static final long serialVersionUID = 1L;
	private Integer key;
	private Message<String, Void> data;
	
	public PingPongMessage(Integer key, String innerMessageKey) {
		this.key = key;
		data = new InnerMessage(innerMessageKey);
	}
	
	@Override
	public Integer getKey() {
		return key;
	}

	@Override
	public Message<String, Void> getData() {
		return data;
	}
	
	@Override
	public String toString() {
		return "key = " + key + " data = " + getData();
	}
	

	public static byte[] toByteArray(Object obj) throws IOException {
		byte[] bytes = null;
		
        try(ByteArrayOutputStream bos = new ByteArrayOutputStream();
        	ObjectOutputStream oos = new ObjectOutputStream(bos)){
            oos.writeObject(obj);
            bytes = bos.toByteArray();
        }
        
        return bytes;
    }
	
	public static Object toObject(byte[] bytes) throws ClassNotFoundException, IOException {
		Object obj = null;
        try(ByteArrayInputStream bis = new ByteArrayInputStream(bytes);
        	ObjectInputStream ois = new ObjectInputStream(bis);) {
        	
            obj = ois.readObject();
        }

        return obj;
	}
	
	public static class InnerMessage implements Message<String, Void>, Serializable {
		private static final long serialVersionUID = 1L;
		String innerKey;
		
		public InnerMessage(String innerKey) {
			this.innerKey = innerKey;
		}
		
		@Override
		public String getKey() {
			return innerKey;
		}
		
		@Override
		public Void getData() {
			return null;
		}
		
		public String toString() {
			return getKey();
		}
	}
}