package il.co.ilrd.filedatabase;

public interface CRUD<K, D> {
	K create(D data);
	D read(K key);
	void update(K key, D data);
	void delete(K key);
}