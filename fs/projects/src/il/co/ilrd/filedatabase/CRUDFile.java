package il.co.ilrd.filedatabase;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Objects;

public class CRUDFile implements CRUD<Integer, String> {
	private File file;
	
	CRUDFile(File fileName) throws IOException {
		file = fileName;
	}
	@Override
	public Integer create(String data){

		try {
			Writer output = new BufferedWriter(new FileWriter(file, true));
			output.append(Objects.requireNonNull(data));
			output.append(System.lineSeparator());
			output.close();
			
			return (int) Files.lines(file.toPath()).count();
		} 
		catch (IOException e) {
			e.printStackTrace();
			System.err.println("Create Failed, IO Exception");
			return (-1);
		}
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