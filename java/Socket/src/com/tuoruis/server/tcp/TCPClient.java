package com.tuoruis.server.tcp;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class TCPClient {
	public static void start() {
		try (Scanner scanner = new Scanner(System.in);Socket socket = new Socket("localhost", 9999);
				OutputStream outputStream = socket.getOutputStream();
				PrintWriter printWriter = new PrintWriter(outputStream);) {
			String line;
			while ((line = scanner.nextLine()) != null) {
				printWriter.println(line);
				printWriter.flush();
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}
