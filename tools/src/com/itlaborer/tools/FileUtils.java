package com.itlaborer.tools;

import java.io.File;
import java.io.FileWriter;

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
}
