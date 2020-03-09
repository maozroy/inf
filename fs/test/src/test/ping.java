package test;

class ping implements Runnable{

	@Override
	public void run() {
		while(true) {
		try {
			pingpong.pingS.acquire();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println(" ping");
//		try {
//			Thread.sleep(1000);
//		} catch (InterruptedException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
		pingpong.pongS.release();
		
	}
	}
	
}