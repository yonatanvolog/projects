package il.co.ilrd.composite;

import java.util.ArrayList;
import java.io.*;

public class Tree {
	FolderT rootFolder;
	public Tree(String path) {
		rootFolder = new FolderT(path);
	}
	
	public void print() {
		rootFolder.print(0);
	}
	
	/****************************************************/
	
	private abstract class FileComponent {
		String name;
		public abstract void print(int indentation);
	}
	
	/****************************************************/

	private class FolderT extends FileComponent {
		ArrayList<FileComponent> list = new ArrayList<>();
		
		public FolderT(String path) {
			File file = new File(path);
			populateFileComponentList(file.listFiles());
			name = file.getName();
		}
		
		private void populateFileComponentList (File[] fileList) {
			for(File iterator : fileList) {
				if(iterator.isFile()) {
					list.add(new FileT(iterator.getName()));
				}
				else {
					list.add(new FolderT(iterator.getAbsolutePath()));
				}
			}
		}
		
		@Override
		public void print(int indentation) {
			System.out.println(getIndent(indentation) + name);

			for (FileComponent fileComponent : list) {
				fileComponent.print(indentation + 2);
			}
		}
	}
	
	/****************************************************/

	private class FileT extends FileComponent {
		public FileT(String path) {
			name = path;
		}

		@Override
		public void print(int indentation) {
			System.out.println(getIndent(indentation) + name);
		}
	}
	
	private StringBuilder getIndent(int indentation) {
		StringBuilder indent = new StringBuilder("_");
		for (int i = 0; i < indentation; ++i) {
			indent.append("_");
		}

		return indent;
	}
}