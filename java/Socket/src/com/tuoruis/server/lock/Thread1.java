package com.tuoruis.server.lock;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Thread1 extends Thread{
	static Lock lock = new ReentrantLock();
	private String str;
	private int index;
	public Thread1(String str)
	{
		this.str = str;
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		super.run();
			lock.lock();
			while(index < 1000)
			{
				index++;
				System.out.println("str: " + str);
				System.out.println("index: " + index);
			}
			lock.unlock();
	}
	

}
