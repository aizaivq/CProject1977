package com.tuoruis.server.udp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class UDPServer {

	public static void start()
	{
		byte[] buff = new byte[1024];
		DatagramPacket datagramPacket = new DatagramPacket(buff, 1024);
	    try (DatagramSocket datagramSocket = new DatagramSocket(1111);
	    		){
	    	while(datagramPacket.getLength() != 0)
	    	{
	    		datagramSocket.receive(datagramPacket);
	    		System.out.println(new String(datagramPacket.getData()));
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
}
