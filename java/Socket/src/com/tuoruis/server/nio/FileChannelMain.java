package com.tuoruis.server.nio;

public class FileChannelMain {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		FileChannelWriter.start("/home/tuoruis/tmp/file2", "wcnmb");
		FileChannelReader.start("/home/tuoruis/tmp/file2");
	}

}
