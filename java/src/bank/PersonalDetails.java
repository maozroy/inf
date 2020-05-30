package bank;

import java.sql.Date;

public class PersonalDetails {
	int id;
	double baseSalary;
	Date birthDate;
	Date startingDate;
	String lastname;
	public PersonalDetails(int id, double baseSalary, Date birthDate, Date startingDate, String lastname) {
		this.id = id;
		this.baseSalary = baseSalary;
		this.birthDate = birthDate;
		this.startingDate = startingDate;
		this.lastname = lastname;
	}
	

}
