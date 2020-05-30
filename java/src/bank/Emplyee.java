package bank;

public class Emplyee {
	PersonalDetails worker;
	SpecialAcitvities isSignee;
	Position pos;
	ExperienceBonus expBonus;
	
	public Emplyee(PersonalDetails worker, SpecialAcitvities isSignee, Position pos, ExperienceBonus expBonus) {
		this.worker = worker;
		this.isSignee = isSignee;
		this.pos = pos;
		this.expBonus = expBonus;
	}

	double CalcYearlySalary()
	{
		return  pos.calcYearlySalary(worker, isSignee) + expBonus.calcExperience(worker);
	}

}