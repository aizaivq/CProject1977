package com.tuoruis.server.nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class SocketChannelWriter {
	public static void start(String data)
	{
		try(SocketChannel socketChannel = SocketChannel.open();)
		{
			socketChannel.connect(new InetSocketAddress("localhost", 7777));
			ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
			byteBuffer.clear();
			byteBuffer.put(data.getBytes());
			byteBuffer.flip();
			socketChannel.write(byteBuffer);
		}
		catch(IOException e)
		{
			
		}
	}
}
