DROP DATABASE tadiran;

CREATE DATABASE tadiran;
USE tadiran;

CREATE TABLE PaymentHistory (
	PaymentHistoryID int AUTO_INCREMENT PRIMARY KEY,
	FK_PaymentID int NOT NULL,
    Date date,
    Amount int NOT NULL,
    Approved boolean NOT NULL
);

CREATE TABLE CompanyContacts (
    CompanyUserID int AUTO_INCREMENT PRIMARY KEY,
    FK_ContactID int NOT NULL
);

CREATE TABLE IOTToUserContact (
    ID int AUTO_INCREMENT PRIMARY KEY,
    FK_serialNumber varchar(16) NOT NULL,
    FK_ContactID int NOT NULL
);

CREATE TABLE IOTEvent (
    IOEventID int AUTO_INCREMENT PRIMARY KEY,
    FK_EventTypeID int NOT NULL,
    TimeStamp TimeStamp NOT NULL,
    FK_serialNumber varchar(16) NOT NULL
);

CREATE TABLE EventType (
    EventTypeID int AUTO_INCREMENT PRIMARY KEY,
    EventName varchar(40) NOT NULL UNIQUE,
    Description varchar(80)
);

CREATE TABLE IOTItem (
    serialNumber varchar(16) PRIMARY KEY,
    FK_ProductID int NOT NULL
);

CREATE TABLE Product (
    ProductID int AUTO_INCREMENT PRIMARY KEY,
    ProductName varchar(20) NOT NULL UNIQUE,
    Description varchar(80)
);

CREATE TABLE Contact (
    ContactID int AUTO_INCREMENT PRIMARY KEY,
    firstName varchar(20) NOT NULL,
    lastName varchar(40) NOT NULL,
    email varchar(20) NOT NULL UNIQUE,
    phone varchar(20) NOT NULL UNIQUE,
    FK_addressID int
);

CREATE TABLE PaymentDetails (
    PaymentID int AUTO_INCREMENT PRIMARY KEY,
    FK_CreditCardNumber varchar(16),
    FK_BillingAddress int NOT NULL
);

CREATE TABLE CardDetails (
    CreditCardNumber varchar(16) PRIMARY KEY,
    FK_CCComapnyID int NOT NULL UNIQUE,
    CreditCardHolderID int NOT NULL,
    ExpireDate Date,
    SecurityCode int
);

CREATE TABLE CreditCardCompany (
    CCCompanyID int AUTO_INCREMENT PRIMARY KEY,
    companyName varchar(40) UNIQUE
);

CREATE TABLE Address (
    addressID int AUTO_INCREMENT PRIMARY KEY,
    ZIP varchar(10),
    address varchar(80) NOT NULL,
    FK_cityID int NOT NULL
);

CREATE TABLE City (
    cityID int AUTO_INCREMENT PRIMARY KEY,
    cityName varchar(40) NOT NULL,
    FK_countryID int NOT NULL
);

CREATE TABLE Country (
    countryID int AUTO_INCREMENT PRIMARY KEY,
    CountryName varchar(20) NOT NULL UNIQUE
);

ALTER TABLE PaymentHistory 
ADD FOREIGN KEY (FK_PaymentID) REFERENCES PaymentDetails(PaymentID);

ALTER TABLE CompanyContacts 
ADD FOREIGN KEY (FK_ContactID) REFERENCES Contact(ContactID);

ALTER TABLE IOTToUserContact 
ADD FOREIGN KEY (FK_serialNumber) REFERENCES IOTItem(serialNumber),
ADD FOREIGN KEY (FK_ContactID) REFERENCES Contact(ContactID);

ALTER TABLE IOTEvent 
ADD FOREIGN KEY (FK_EventTypeID) REFERENCES EventType(EventTypeID),
ADD FOREIGN KEY (FK_serialNumber) REFERENCES IOTItem(serialNumber);

ALTER TABLE IOTItem 
ADD FOREIGN KEY (FK_ProductID) REFERENCES Product(ProductID);

ALTER TABLE Contact 
ADD FOREIGN KEY (FK_addressID) REFERENCES Address(addressID);

ALTER TABLE PaymentDetails 
ADD FOREIGN KEY (FK_CreditCardNumber) REFERENCES CardDetails(CreditCardNumber),
ADD FOREIGN KEY (FK_BillingAddress) REFERENCES Address(addressID);

ALTER TABLE CardDetails 
ADD FOREIGN KEY (FK_CCComapnyID) REFERENCES CreditCardCompany(CCCompanyID);

ALTER TABLE Address
ADD FOREIGN KEY (FK_cityID) REFERENCES City(cityID);

ALTER TABLE City 
ADD FOREIGN KEY (FK_countryID) REFERENCES Country(countryID);

INSERT into Country values(1, "Israel"), (2, "USA");
INSERT into City values(1, "Ramat Gan", 1), (2, "Texas", 2);
INSERT into Address values(1, "11111" ,"Haatzamaut 12", 1), (2, "22222", "Trump st. 99", 2);
INSERT into CreditCardCompany values(1, "Visa");
INSERT into CardDetails values("1234678910111213" , 1, 32331, "09-04-25", 123);
INSERT into EventType values(1, "Software update", "ver 1.2");
INSERT into Product values(1, "Alpha", "Best Air Cond");
INSERT into Contact values(1, "Israel", "Israeli", "israel@gmail.com", "0521111111", 1);
INSERT into PaymentDetails values(1, "1234678910111213", 1);
INSERT into IOTItem values("00001", 1);
INSERT into PaymentHistory values(1, 1, "14-03-20", 1, false), (2, 1, "15-03-20", 1, true);
INSERT into CompanyContacts values(1, 1);
INSERT into IOTToUserContact values(1, "00001", 1);
INSERT into IOTEvent values(1, 1, '2020-03-17 12:31:01.123415',"00001");
