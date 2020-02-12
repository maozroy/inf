package bank;

public class Manager implements Position {
	
	private double managerBonus;
	
	public double getManagerBonus() {
		return managerBonus;
	}

	public void setManagerBonus(double managerBonus) {
		this.managerBonus = managerBonus;
	}

	@Override
	public double calcYearlySalary(PersonalDetails worker, SpecialAcitvities signee) {
		return (signee.calcBonus(worker)*12) + managerBonus;
	}

}
