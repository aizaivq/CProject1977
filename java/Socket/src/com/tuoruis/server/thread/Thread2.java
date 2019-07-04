package com.tuoruis.server.thread;

public class Thread2 extends Thread{
	private DataBean dataBean;
	public Thread2(DataBean dataBean)
	{
		this.dataBean = dataBean;
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		super.run();
		try {
			sleep(2000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		synchronized (dataBean) {
			dataBean.setAge(20);
			dataBean.notify();
		}
	}
	

}
