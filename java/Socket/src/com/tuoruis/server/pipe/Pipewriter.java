package com.tuoruis.server.pipe;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.Scanner;

public class Pipewriter {
	public static PipedOutputStream pipedOutputStream = new PipedOutputStream();
	public static void start(PipedInputStream pipedInputStream)
	{
		try(Scanner scanner = new Scanner(System.in))
		{
	//		pipedOutputStream.connect(pipedInputStream);
			String line;
			while((line = scanner.nextLine()) != null)
			{
				pipedOutputStream.write(line.getBytes());
				pipedOutputStream.flush();
			}
		}
		catch (IOException e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		finally {
			try {
				pipedOutputStream.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}


