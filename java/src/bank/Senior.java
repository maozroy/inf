package bank;

public class Senior implements Position {

	@Override
	public double calcYearlySalary(PersonalDetails worker, SpecialAcitvities signee) {
		return (signee.calcBonus(worker) * 12) * 1.1;
	}

}
