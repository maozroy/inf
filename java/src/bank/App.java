package bank;

import java.sql.Date;

public class App {

	public static void main(String[] args) {
		Emplyee tamir = new Emplyee(
			new PersonalDetails(345345, 1.5, new Date(0), new Date(1900, 1, 2), "Dayan"),
			new NoSignee(),
			new Senior(),
			new YearExp()
			);
		
		System.out.println(tamir.CalcYearlySalary());
	}

}
