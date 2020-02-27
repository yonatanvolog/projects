package il.co.ilrd.factory_tree;

import java.util.ArrayList;
import java.util.function.Function;

import il.co.ilrd.factory.*;
import java.io.*;
import java.nio.file.Paths;

public class FactoryTree {
	FolderT rootFolder;
	static Factory<FileComponent, Boolean, String> componentFactory;
	
	public FactoryTree(String path) {
		rootFolder = new FolderT(path);
	}
	
	public void print() {
		rootFolder.print(0);
	}
	
	{
		componentFactory = new Factory<>();
		Function<String, FileComponent> createFile =  (str) -> new FileT(str);
		Function<String, FileComponent> createFolder = (str) -> new FolderT(str);
		
		componentFactory.add(true, createFile);
		componentFactory.add(false, createFolder);
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
			
			for(File iterator : file.listFiles()) {
				list.add(componentFactory.create(iterator.isFile(), iterator.getAbsolutePath()));
			}
			
			name = file.getName();
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
			name =Paths.get(path).toFile().getName();
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