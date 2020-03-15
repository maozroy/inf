package filedatabase;

public interface CRUD<K, D> {
	K Create(D data);
	D Read(K key);
	void update(K key, D data);
	void delete(K key);
}
