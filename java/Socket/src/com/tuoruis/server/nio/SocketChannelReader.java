package com.tuoruis.server.nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class SocketChannelReader {
	public static void start()
	{
		try(SocketChannel socketChannel = SocketChannel.open();)
		{
			socketChannel.connect(new InetSocketAddress("localhost", 7777));
			ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
			while(socketChannel.isConnected())
			{
				if(socketChannel.read(byteBuffer) != -1)
				{
				byteBuffer.flip();
				while(byteBuffer.hasRemaining())
				System.out.println((char)byteBuffer.get());
				byteBuffer.clear();
				}
			}
		}
		catch(IOException e)
		{
			
		}
	}
}
