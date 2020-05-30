package bank;

public class NoExp implements ExperienceBonus {

	@Override
	public double calcExperience(PersonalDetails worker) {
		return 0;
	}

}
