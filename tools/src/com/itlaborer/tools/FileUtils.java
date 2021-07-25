package com.itlaborer.tools;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class FileUtils {
	// String保存到文件方法
	public static boolean saveToFile(File file, String content) {
		FileWriter fwriter = null;
		try {
			fwriter = new FileWriter(file);
			fwriter.write(content);
			fwriter.flush();
			fwriter.close();
			return true;
		} catch (Exception ex) {
			return false;
		}
	}
	public static boolean fileIsExists(String filepath) {
		File file=new File(filepath);
		return file.exists();
	}
	public static ArrayList<String> readFileToArrayList(String filepath,String encoding) {
		File file=new File(filepath);
		ArrayList<String> list=new ArrayList<String> ();
		FileInputStream inputStream = null;
		try {
			inputStream = new FileInputStream(file.getPath());
			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream, encoding));
			String str = null;
			while ((str = bufferedReader.readLine()) != null) {
				list.add(str);
			}
			return list;
		} catch (Exception e) {
			try {
				inputStream.close();
			} catch (IOException e1) {
			}
		} finally {
			try {
				inputStream.close();
			} catch (IOException e1) {
			}
		}
		return list;
	}
}
