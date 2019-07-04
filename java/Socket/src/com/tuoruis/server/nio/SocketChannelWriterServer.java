package com.tuoruis.server.nio;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

public class SocketChannelWriterServer {

	public static void start() {
		try (
				Scanner scanner = new Scanner(System.in)) {
			ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
			serverSocketChannel.bind(new InetSocketAddress("localhost", 7777));
			SocketChannel socketChannel = serverSocketChannel.accept();
			String line;
			while ((line = scanner.nextLine()) != null) {
				ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
				byteBuffer.clear();
				byteBuffer.put(line.getBytes());
				byteBuffer.flip();
				socketChannel.write(byteBuffer);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}
