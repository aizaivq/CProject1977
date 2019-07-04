package com.tuoruis.server.nio;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class FileChannelWriter {
	public static void start(String fileName,String data) {
		try (RandomAccessFile randomAccessFile = new RandomAccessFile(fileName, "rw");FileChannel fileChannel = randomAccessFile.getChannel();) {
			ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
			byteBuffer.clear();
			byteBuffer.put(data.getBytes());
			byteBuffer.flip();
			fileChannel.write(byteBuffer);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
