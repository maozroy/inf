package il.co.ilrd.server.pingpong;

import il.co.ilrd.server.general.*;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

import il.co.ilrd.server.general.Message;

public class PingPongMessage implements Message<String, Void>, Serializable {
	String data;
	
	
	public PingPongMessage(String data) {
		this.data = data;
	}

	@Override
	public String getKey() {
		return data;
	}

	@Override
	public Void getData() {
		return null;
	}
	
	public String toString() {
		return getKey();
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
}
