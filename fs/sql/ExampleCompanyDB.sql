CREATE database Company_Template;
USE Company_Template;




CREATE TABLE Country(
CountryID INT AUTO_INCREMENT PRIMARY KEY,

CountryName VARCHAR(20)

);

CREATE TABLE City(
CityID INT AUTO_INCREMENT PRIMARY KEY,

CityName VARCHAR(40),
FK_CountryID int,
CONSTRAINT FK_CountryID FOREIGN KEY(FK_CountryID)
REFERENCES Country(CountryID)

);

CREATE TABLE Address(
AddressID INT AUTO_INCREMENT PRIMARY KEY,

ZIP VARCHAR(10),

Address VARCHAR(80),
FK_CityID int,
CONSTRAINT FK_CityID FOREIGN KEY(FK_CityID)
REFERENCES City(CityID)

);

CREATE TABLE Contact(
ContactID INT AUTO_INCREMENT PRIMARY KEY,
FirstName VARCHAR(20),
LastName VARCHAR(20),
email VARCHAR(80),
phone VARCHAR(15),
FK_AddressID int,
CONSTRAINT FK_AddressID FOREIGN KEY(FK_AddressID)
REFERENCES Address(AddressID)

);

CREATE TABLE CreditCardCompany(
	CCCompanyID int AUTO_INCREMENT PRIMARY KEY,
    
    CompanyName VARCHAR(20)
    
);

CREATE TABLE CardDetails(
	CCNumber int AUTO_INCREMENT PRIMARY KEY,
    FK_CCCompanyID int,
	CONSTRAINT FK_CCCompanyID FOREIGN KEY(FK_CCCompanyID)
	REFERENCES CreditCardCompany(CCCompanyID),
    
    CreditCardHolderID VARCHAR(9),
    
    ExpireDate DATE,
    
    SecurityCode VARCHAR(3)
    
);

CREATE TABLE CompanyContact (
	CompanyUserID  int PRIMARY KEY, 
    
    FK_ContactID int,
    CONSTRAINT FK_ContactID FOREIGN KEY (FK_ContactID)
    REFERENCES Contact(ContactID)
); 


CREATE TABLE PaymentDetails(
	PaymentID int AUTO_INCREMENT PRIMARY KEY,

	FK_CCNumber int,

	FK_BillingAddress int,
    
   	CONSTRAINT FK_CCNumber FOREIGN KEY(FK_CCNumber)
	REFERENCES CardDetails(CCNumber),
    
	CONSTRAINT FK_BillingAddress FOREIGN KEY(FK_BillingAddress)
	REFERENCES Address(AddressID)
);

CREATE TABLE PaymentHistory (
	PaymentHistoryID int AUTO_INCREMENT PRIMARY KEY,
    
    Date DATE,
    
    Amount int,
    
    Approved boolean,
    
    FK_PaymentID int,
	CONSTRAINT FK_PaymentID FOREIGN KEY(FK_PaymentID)
    REFERENCES PaymentDetails(PaymentID)
    
);

CREATE TABLE EventType(
	EventTypeID int AUTO_INCREMENT PRIMARY KEY,
	EventName VARCHAR(80),
    Description VARCHAR(80)
);

CREATE TABLE Product(
	ProductID int AUTO_INCREMENT PRIMARY KEY,
    ProductName VARCHAR(80),
    Description VARCHAR(80)
);

CREATE TABLE IOTItem(
	SerialNumber VARCHAR(80) PRIMARY KEY,
	
    FK_ProductID int,
	CONSTRAINT FK_ProductID FOREIGN KEY(FK_ProductID)
    REFERENCES Product(ProductID)

);

CREATE TABLE IOTEvent(
	IOTEventID int AUTO_INCREMENT PRIMARY KEY,
    
    FK_EventTypeID int,
	CONSTRAINT FK_EventTypeID FOREIGN KEY(FK_EventTypeID)
    REFERENCES EventType(EventTypeID),
    
    TimeStamp TIMESTAMP,
    
    FK_SerialNumber VARCHAR(80),
	CONSTRAINT FK_SerialNumber FOREIGN KEY(FK_SerialNumber)
    REFERENCES IOTItem(SerialNumber)
);

CREATE TABLE IOTToUserContact(
	UserContactID INT AUTO_INCREMENT PRIMARY KEY,
    
	FK_SerialNumberIOT VARCHAR(80),
	CONSTRAINT FK_SerialNumberIOT FOREIGN KEY(FK_SerialNumberIOT)
    REFERENCES IOTItem(SerialNumber),
    
    FK_IOTContactID int,
	CONSTRAINT FK_IOTContactID FOREIGN KEY (FK_IOTContactID)
    REFERENCES Contact(ContactID)
);


