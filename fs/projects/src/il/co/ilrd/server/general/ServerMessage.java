package il.co.ilrd.server.general;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class ServerMessage implements Message<ProtocolType, Message<?, ?>>, Serializable {

	private static final long serialVersionUID = 2159575795746945290L;
	ProtocolType key;
	Message<?, ?> message;
	
	public ServerMessage(ProtocolType key, Message<?, ?> message) {
		this.key = key;
		this.message = message;
	}

	@Override
	public ProtocolType getKey() {
		return key;
	}

	@Override
	public Message<?, ?> getData() {
		return message;
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
