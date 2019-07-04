package com.scrlk;

import java.util.concurrent.ThreadFactory;

public class DaemonThreadFactory implements ThreadFactory {
  public Thread newThread(Runnable r) {
	  System.out.println("DaemonThreadFactory");
    Thread t = new Thread(r);
    t.setDaemon(true);
    return t;
  }
  
  
  
}