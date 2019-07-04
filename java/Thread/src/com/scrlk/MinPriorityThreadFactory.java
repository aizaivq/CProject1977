package com.scrlk;

import java.util.concurrent.ThreadFactory;

public class MinPriorityThreadFactory implements ThreadFactory {
  public Thread newThread(Runnable r) {
    Thread t = new Thread(r);
    t.setPriority(Thread.MIN_PRIORITY);
    return t;
  }
}