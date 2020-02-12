package ilrd.vm;

public class VMProduct {
	
	private int amount;
	Product product;
	
	public VMProduct(int amount, Product product) {
		this.amount = amount;
		this.product = product;
	}

	public int getAmount() {
		return amount;
	}

	public void decreaseAmount() {
		--amount;
	}	
	
	public enum Product {
		
		COCA_COLA (4.5),
		DIET_COLA (3.0),
		SPRITE    (3.0),
		FANTA     (3.9),
		FUSE_TEA  (4.0);
		
		private double price;
		
		private Product (double price) {
			this.price = price;			
		}
		public double getPrice() {
			return (price);
		}
	}
	
	
	
	
	
}