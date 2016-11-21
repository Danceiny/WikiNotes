class MyRunnable implements Runnable{
	public void run(){
		go();
	}
	public void go(){
		System.out.println("top of the stack");
	}
}

public class ThreadTestDrive{
	public static void main(String[] args){
		Runnable threadJob = new MyRunnable();
		Thread thread = new Thread(threadJob);
		thread.start();
		System.out.println("back in main");
	}
}