package com.scrlk;

import java.util.concurrent.ThreadFactory;

public class MaxPriorityThreadFactory implements ThreadFactory {
  public Thread newThread(Runnable r) {
	  System.out.println("DaemonThreadFactory");

    Thread t = new Thread(r);
    t.setPriority(Thread.MAX_PRIORITY);
    return t;
  }
}