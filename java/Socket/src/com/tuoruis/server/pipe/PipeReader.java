package com.tuoruis.server.pipe;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class PipeReader {
	public static  PipedInputStream pipedInputStream = new PipedInputStream();
	public static void start(PipedOutputStream pipedOutputStream)
	{
		byte buff[] = new byte[1024];
		try
		{
			pipedInputStream.connect(pipedOutputStream);
			while(pipedInputStream.read(buff) != -1)
			{
				System.out.println(new String(buff));
			}
		}
		catch (IOException e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		finally {
//			try {
//				pipedInputStream.close();
//			} catch (IOException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
		}
	}
}
