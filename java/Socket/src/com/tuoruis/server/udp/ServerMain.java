package com.tuoruis.server.udp;


public class ServerMain {

	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//		   InetAddress inetRemoteAddr = InetAddress.getByName("224.0.0.5");
//	        DatagramPacket recvPack = new DatagramPacket(new byte[1024], 1024);
//	        MulticastSocket server = new MulticastSocket(8888);
//	        server.joinGroup(inetRemoteAddr);
		UDPServer.start();
	}

}
