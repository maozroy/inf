package bank;

public class NoSignee implements SpecialAcitvities {

	@Override
	public double calcBonus(PersonalDetails worker) {
		return worker.baseSalary;
	}

}
