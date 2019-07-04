package com.tuoruis.server.nio;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class FileChannelReader {
	public static void start(String fileName) {
		try (RandomAccessFile randomAccessFile = new RandomAccessFile(fileName, "rw");) {
			FileChannel fileChannel = randomAccessFile.getChannel();
			ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
			while (fileChannel.read(byteBuffer) != -1) {
				byteBuffer.flip();
				while (byteBuffer.hasRemaining()) {
					System.out.println((char)byteBuffer.get());
				}
				byteBuffer.clear();
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
