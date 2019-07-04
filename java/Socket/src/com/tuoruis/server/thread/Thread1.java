package com.tuoruis.server.thread;

public class Thread1 extends Thread{
	private DataBean dataBean;
	public Thread1(DataBean dataBean)
	{
		this.dataBean = dataBean;
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		synchronized (dataBean) {
			if(dataBean.getAge() < 10)
			{
				try {
					System.out.println("age < 10 wait..");
					dataBean.wait();
					System.out.println("dataBean notify age: " + dataBean.getAge());
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			else
			{
				System.out.println("age > 10");
			}
		}
		super.run();
	}
	

}
