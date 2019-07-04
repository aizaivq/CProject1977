package com.tuoruis.server.udp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.util.Scanner;

public class UDPClient {
	public static void start() {
		byte[] buff = new byte[1024];
		DatagramPacket datagramPacket = new DatagramPacket(buff, 1024,new InetSocketAddress("localhost", 1111));
	    try (DatagramSocket datagramSocket = new DatagramSocket(2222);
		    	Scanner scanner = new Scanner(System.in);
	    		){
	    	String line;
	    	while((line = scanner.nextLine()) != null)
	    	{
	    		datagramPacket.setData(line.getBytes());
	    		datagramSocket.send(datagramPacket);
	    	}
			System.out.println(new String(datagramPacket.getData()));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
}
