package il.co.ilrd.tree;

import java.nio.file.Paths;
import java.util.ArrayList;

import java.io.*;

public class Tree {
	FolderT rootFolderT;
	private static final int INDEN_PRESET = 4;
	
	public Tree(String path) {
		rootFolderT = new FolderT(path);
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
				if(fileInFolder.isFile()) {
					list.add(new FileT(fileInFolder.getAbsolutePath()));
				}
				else {
					list.add(new FolderT(fileInFolder.getAbsolutePath()));
				}
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