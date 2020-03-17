package il.co.ilrd.filedatabase;

import java.io.File;

public class CRUDFile implements CRUD<Integer, String> {
	private File file;
	
	CRUDFile(String fileName) {
		file = new File(fileName);
	}
	
	@Override
	public Integer create(String data) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String read(Integer key) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void update(Integer key, String newData) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void delete(Integer key) {
		// TODO Auto-generated method stub
		
	}
}