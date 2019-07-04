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

public class SocketChannelReaderServer {

	public static void start()
	{
		try {
			ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
			serverSocketChannel.bind(new InetSocketAddress("localhost", 7777));
			SocketChannel socketChannel = serverSocketChannel.accept();
			ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
			while(socketChannel.read(byteBuffer) != -1)
			{
				byteBuffer.flip();
				while(byteBuffer.hasRemaining())
				System.out.println((char)byteBuffer.get());
				byteBuffer.clear();
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
