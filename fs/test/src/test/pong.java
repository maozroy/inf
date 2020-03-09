package test;

class pong implements Runnable{

	@Override
	public void run() {
		while(true) {
		try {
			pingpong.pongS.acquire();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("pong");
//		try {
//			Thread.sleep(1000);
//		} catch (InterruptedException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
		pingpong.pingS.release();
		
	}
	}
	
}