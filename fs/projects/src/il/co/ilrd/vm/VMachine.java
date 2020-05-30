package il.co.ilrd.vm;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

import il.co.ilrd.vm.VMProduct.Product;

public class VMachine implements Runnable {

	private double currentMoney;
	private States currentState;	
	private Monitor curMonitor = new TestMonitor();
	int timeoutCounter = 0;
	static final int threshold = 5; 
	private Thread timeoutThread = new Thread(this);
	private AtomicBoolean isAlive = new AtomicBoolean(false);
	VMProduct[] productInMachine = new VMProduct[5];

	public VMachine() {
		setCurrentMoney(0);
		currentState = States.INIT;	
		productInMachine[0] = new VMProduct(2, VMProduct.Product.COCA_COLA);
		productInMachine[1] = new VMProduct(10, VMProduct.Product.DIET_COLA);
		productInMachine[2] = new VMProduct(5, VMProduct.Product.SPRITE);
		productInMachine[3] = new VMProduct(8, VMProduct.Product.FANTA);
		productInMachine[4] = new VMProduct(15, VMProduct.Product.FUSE_TEA);
	}
	
	public void addMoney(double money) {
		currentState.insertMoney(this, money);
	}
	
	public void selectItem(Product name) {		
		currentState.selectProduct(this, name);		
	}
	
	public void getRefund() {
		currentState.askForRefund(this);		
	}
	
	public void turnOn() {
		currentState.start(this);	
	}
	
	public void turnOff() {
		currentState.turnOff(this);		
	}

	public double getCurrentMoney() {
		return currentMoney;
	}

	private void setCurrentMoney(double currentMoney) {
		this.currentMoney = currentMoney;
	}
	private enum States {
		INIT {
			@Override
			public void start(VMachine VM) {
				VM.curMonitor.print("Machine Started");
				VM.isAlive.set(true);
				VM.timeoutThread.start();
				VM.currentState = WAIT_FOR_MONEY;			
			}

			@Override
			public void insertMoney(VMachine VM, double money) {
				VM.curMonitor.print("Machine Not Started");
			}

			@Override
			public void selectProduct(VMachine VM, Product name) {
				VM.curMonitor.print("Machine Not Started");
			}

			@Override
			public void askForRefund(VMachine VM) {
				VM.curMonitor.print("Machine Not Started");
			}

			@Override
			public void turnOff(VMachine VM) {
				VM.curMonitor.print("Machine Not Started");
			}		
		},
		WAIT_FOR_MONEY {
			@Override
			public void selectProduct(VMachine VM, Product name) {
				VM.curMonitor.print("First insert Money!");
			}

			@Override
			public void askForRefund(VMachine VM) {
				VM.curMonitor.print("First insert Money!");
			}			
		},
		WAIT_FOR_CHOOSING {
			
			@Override
			public void checkCounter(VMachine VM) {
				++VM.timeoutCounter;
				if (threshold <= VM.timeoutCounter) {
					VM.currentState.askForRefund(VM);
				}
			}
			
			@Override
			public void selectProduct(VMachine VM, Product name) {
				int productIndex = name.ordinal();
				double price = VM.productInMachine[productIndex].product.getPrice();
				
				if(0 >= VM.productInMachine[productIndex].getAmount()) {
					VM.curMonitor.print(name + " ran out");
				}
				else {
					if(price <= VM.currentMoney) {
						VM.currentMoney -= price;
						VM.productInMachine[productIndex].decreaseAmount();
						VM.curMonitor.print("Enjoy your " + name);
					}else {
						VM.curMonitor.print("You Don't Have Enough for " + name);
					}
				}
				askForRefund(VM);
			}

			@Override
			public void askForRefund(VMachine VM) {
				VM.curMonitor.print(VM. getCurrentMoney() + " Is Withrawn");
				VM.setCurrentMoney(0);
				VM.currentState = WAIT_FOR_MONEY;
			}
		};
		
		public void insertMoney(VMachine VM, double money) {
			VM.currentMoney += money;
			VM.currentState = WAIT_FOR_CHOOSING;
			VM.timeoutCounter = 0;

		}
		public abstract void selectProduct(VMachine VM, Product name);
		public abstract void askForRefund(VMachine VM);
		public void start(VMachine VM) {
			VM.curMonitor.print("Machine Already Started");
		}
		public void turnOff(VMachine VM) {
			VM.curMonitor.print("Machine Shutting Down");
			askForRefund(VM);
			VM.currentState = INIT;
			VM.isAlive.set(false);
		}
		public void checkCounter(VMachine VM) {
			VM.timeoutCounter = 0;
		}
	}
	
		@Override
		public void run() {
			while(isAlive.get()) {
				try {
					TimeUnit.SECONDS.sleep(	1);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				currentState.checkCounter(VMachine.this);
			}
		}
}