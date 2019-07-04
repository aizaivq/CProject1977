package com.tuoruis.server.pipe;

public class PiperMain {
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Thread thread = new Thread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				PipeReader.start(Pipewriter.pipedOutputStream);
			}
		});
		thread.start();
		Thread thread1 = new Thread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Pipewriter.start(PipeReader.pipedInputStream);

			}
		});
		thread1.start();
	}
}

