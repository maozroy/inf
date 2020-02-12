package bank;

public class Signee implements SpecialAcitvities {

	@Override
	public double calcBonus(PersonalDetails worker) {
		return (worker.baseSalary * 1.1);
	}

}
