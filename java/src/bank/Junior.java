package bank;

public class Junior implements Position {

	@Override
	public double calcYearlySalary(PersonalDetails worker, SpecialAcitvities signee) {
		return (signee.calcBonus(worker) * 12);
	}

}
