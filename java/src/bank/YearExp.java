package bank;

public class YearExp implements ExperienceBonus {

	@Override
	public double calcExperience(PersonalDetails worker) {
		return worker.baseSalary;
	}

}
