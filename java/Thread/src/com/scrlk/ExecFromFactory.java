package com.scrlk;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
   
public class ExecFromFactory {
  public static void main(String[] args) throws Exception {
    ExecutorService defaultExec = Executors.newCachedThreadPool();
   
    ExecutorService daemonExec = Executors
       .newCachedThreadPool(new DaemonThreadFactory());
    ExecutorService maxPriorityExec = Executors
       .newCachedThreadPool(new MaxPriorityThreadFactory());
    ExecutorService minPriorityExec = Executors
       .newCachedThreadPool(new MinPriorityThreadFactory());
    for (int i = 0; i < 10; i++)
      daemonExec.execute(new MyThread(i));
    for (int i = 10; i < 20; i++)
      if (i == 10)
       maxPriorityExec.execute(new MyThread(i));
      else if (i == 11)
       minPriorityExec.execute(new MyThread(i));
      else
       defaultExec.execute(new MyThread(i));
  }
}