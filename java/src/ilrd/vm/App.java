package ilrd.vm;


import ilrd.vm.VMProduct.Product;

public class App {

	public static void main(String[] args) throws InterruptedException {
		VMachine my_vendingMachine = new VMachine();
		my_vendingMachine.turnOn();

		my_vendingMachine.addMoney(30);
		Thread.sleep(6000);
		my_vendingMachine.selectItem(Product.COCA_COLA);
		my_vendingMachine.addMoney(25);
		Thread.sleep(1000);
		my_vendingMachine.selectItem(Product.COCA_COLA);
		my_vendingMachine.addMoney(5);
		Thread.sleep(1000);

		my_vendingMachine.selectItem(Product.COCA_COLA);
		my_vendingMachine.addMoney(5);
		my_vendingMachine.selectItem(Product.COCA_COLA);
		my_vendingMachine.addMoney(5);
		Thread.sleep(1000);

		my_vendingMachine.selectItem(Product.COCA_COLA);
		my_vendingMachine.addMoney(2);
		my_vendingMachine.selectItem(Product.COCA_COLA);
		my_vendingMachine.addMoney(2);
		my_vendingMachine.addMoney(2);
		System.out.println("check");

		my_vendingMachine.addMoney(2);
		my_vendingMachine.turnOff();
//		my_vendingMachine.selectItem(Product.DIET_COLA);
//		my_vendingMachine.getRefund();





		
	}

}
