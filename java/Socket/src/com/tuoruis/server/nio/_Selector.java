package com.tuoruis.server.nio;

import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

public class _Selector {
	public static void start(SocketChannel socketChannel)
	{
		try
		{
		Selector selector = Selector.open();
		socketChannel.configureBlocking(false);
		SelectionKey key = socketChannel.register(selector, SelectionKey.OP_READ);
		while(true) {
		  int readyChannels = selector.select();
		  if(readyChannels == 0) continue;
		  Set selectedKeys = selector.selectedKeys();
		  Iterator keyIterator = selectedKeys.iterator();
		  while(keyIterator.hasNext()) {
		    key = (SelectionKey) keyIterator.next();
		    if(key.isAcceptable()) {
		        // a connection was accepted by a ServerSocketChannel.
		    } else if (key.isConnectable()) {
		        // a connection was established with a remote server.
		    } else if (key.isReadable()) {
		        // a channel is ready for reading
		    } else if (key.isWritable()) {
		        // a channel is ready for writing
		    }
		    keyIterator.remove();
		  }
		}

		}
		catch (Exception e) {
			// TODO: handle exception
		}
	}
}
