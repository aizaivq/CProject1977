package com.tuoruis.server.lock;


public class LockMain {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
			Thread1 thread1 = new Thread1("thread1");
			Thread1 thread2 = new Thread1("thread2");
			thread1.start();
			thread2.start();

	}

}
