package il.co.ilrd.filedatabase;

import java.io.FileNotFoundException;
import java.io.IOException;

public  interface CRUD<K, D> {
	K create(D data) throws FileNotFoundException, IOException;
	D read(K key);
	void update(K key, D data);
	void delete(K key);
}
