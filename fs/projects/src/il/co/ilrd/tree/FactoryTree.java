package il.co.ilrd.tree;

import java.io.File;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.function.Function;


import il.co.ilrd.factory.Factory;




public class FactoryTree {
	FolderT rootFolderT;
	private static final int INDEN_PRESET = 4;
	Factory<FileComponent, Boolean, String> fileFactory;
	{
		initFactory();
	}
	public FactoryTree(String path) {
		rootFolderT = new FolderT(path);

	}
	
	private void initFactory() {
		fileFactory = new Factory<>();
		fileFactory.add(false,(pathT) -> new FolderT(pathT));
		fileFactory.add(true,(pathT) -> new FileT(pathT));		
	}

	public void print() {
		rootFolderT.print(0);
	}
	
	/****************************************************/
	
	private abstract class FileComponent {
		String name;
		public abstract void print(int indentation);
	}
	
	/****************************************************/

	private class FolderT extends FileComponent {

		ArrayList<FileComponent> list = new ArrayList<FileComponent>();
		
		public FolderT(String path) {
			File[] fileList = Paths.get(path).toFile().listFiles();
			name = Paths.get(path).toFile().getName();
			
			for(File fileInFolder : fileList) {
				list.add(fileFactory.create(fileInFolder.isFile(), fileInFolder.getAbsolutePath()));
			}
		}

		@Override
		public void print(int indentation) {
			System.out.println(indent(indentation, false) + name);
			for (FileComponent filename : list) {
				filename.print(indentation + INDEN_PRESET);
			}
		}
	}
	
	/****************************************************/

	private class FileT extends FileComponent {
			
		public FileT(String path) {
			name = Paths.get(path).toFile().getName();
		}

		@Override
		public void print(int indentation) {
			System.out.println(indent(indentation, true) + name);
		}
	}
	
	private StringBuilder indent (int numberOfSpaces, boolean isFile) {
		StringBuilder spaceString = new StringBuilder();
		int i = 0;
		
		for (i = 0; i < numberOfSpaces - INDEN_PRESET; i++) {			
				spaceString.append(" ");
		}
		if (isFile) {spaceString.append("|");}	

		for (; i < numberOfSpaces; i++) {			
				spaceString.append("_");	
		}
		
		return spaceString;
	}
}