package com.tuoruis.server.thread;

public class ThreadMain {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		DataBean dataBean = new DataBean();
		dataBean.setAge(1);
		Thread1 thread1 = new Thread1(dataBean);
		Thread2 thread2 = new Thread2(dataBean);
		thread2.start();
		thread1.start();
	}

}
