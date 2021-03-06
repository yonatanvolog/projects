DROP DATABASE tadiran;

CREATE DATABASE tadiran;
USE tadiran;

CREATE TABLE PaymentHistory (
	payment_history_id int AUTO_INCREMENT PRIMARY KEY,
	payment_id int NOT NULL,
    payment_date date,
    amount int NOT NULL,
    approved boolean NOT NULL
);

CREATE TABLE CompanyContacts (
    company_user_id int AUTO_INCREMENT PRIMARY KEY,
    contact_id int NOT NULL
);

CREATE TABLE IOTToUserContact (
    id int AUTO_INCREMENT PRIMARY KEY,
    serial_number varchar(16) NOT NULL,
    contact_id int NOT NULL
);

CREATE TABLE IOTEvent (
    iot_event_id int AUTO_INCREMENT PRIMARY KEY,
	serial_number varchar(16) NOT NULL,
    description varchar(255) NOT NULL,
    event_timestamp TimeStamp DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IOTItem (
    serial_number varchar(16) PRIMARY KEY,
    product_id int NOT NULL
);

CREATE TABLE Product (
    product_id int AUTO_INCREMENT PRIMARY KEY,
    product_name varchar(20) NOT NULL UNIQUE,
    description varchar(80)
);

CREATE TABLE Contact (
    contact_id int AUTO_INCREMENT PRIMARY KEY,
    first_name varchar(20) NOT NULL,
    last_name varchar(40) NOT NULL,
    email varchar(20) NOT NULL UNIQUE,
    phone varchar(20) NOT NULL UNIQUE,
    address_id int
);

CREATE TABLE PaymentDetails (
    payment_id int AUTO_INCREMENT PRIMARY KEY,
    credit_card_number varchar(16),
    billing_address int NOT NULL
);

CREATE TABLE CardDetails (
    credit_card_number varchar(16) PRIMARY KEY,
    cc_comapny_id int NOT NULL UNIQUE,
    credit_card_holder_id int NOT NULL,
    expire_date Date,
    security_code int
);

CREATE TABLE CreditCardCompany (
    cc_company_id int AUTO_INCREMENT PRIMARY KEY,
    company_name varchar(40) UNIQUE
);

CREATE TABLE Address (
    address_id int AUTO_INCREMENT PRIMARY KEY,
    zip varchar(10),
    address varchar(80) NOT NULL,
    city_id int NOT NULL
);

CREATE TABLE City (
    city_id int AUTO_INCREMENT PRIMARY KEY,
    city_name varchar(40) NOT NULL,
    country_id int NOT NULL
);

CREATE TABLE Country (
    country_id int AUTO_INCREMENT PRIMARY KEY,
    country_name varchar(20) NOT NULL UNIQUE
);

ALTER TABLE PaymentHistory 
ADD FOREIGN KEY (payment_id) REFERENCES PaymentDetails(payment_id);

ALTER TABLE CompanyContacts 
ADD FOREIGN KEY (contact_id) REFERENCES Contact(contact_id);

ALTER TABLE IOTToUserContact 
ADD FOREIGN KEY (serial_number) REFERENCES IOTItem(serial_number),
ADD FOREIGN KEY (contact_id) REFERENCES Contact(contact_id);

ALTER TABLE IOTEvent 
ADD FOREIGN KEY (serial_number) REFERENCES IOTItem(serial_number);

ALTER TABLE IOTItem 
ADD FOREIGN KEY (product_id) REFERENCES Product(product_id);

ALTER TABLE Contact 
ADD FOREIGN KEY (address_id) REFERENCES Address(address_id);

ALTER TABLE PaymentDetails 
ADD FOREIGN KEY (credit_card_number) REFERENCES CardDetails(credit_card_number),
ADD FOREIGN KEY (billing_address) REFERENCES Address(address_id);

ALTER TABLE CardDetails 
ADD FOREIGN KEY (cc_comapny_id) REFERENCES CreditCardCompany(cc_company_id);

ALTER TABLE Address
ADD FOREIGN KEY (city_id) REFERENCES City(city_id);

ALTER TABLE City 
ADD FOREIGN KEY (country_id) REFERENCES Country(country_id);

INSERT into Country values(null, "Israel1"), (null, "USA1");
INSERT into City values(null, "Ramat Gan", 1), (null, "Texas", 2);
INSERT into Address values(null, "11111" ,"Haatzamaut 12", 1), (null, "22222", "Trump st. 99", 2);
INSERT into CreditCardCompany values(null, "Visa");
INSERT into CardDetails values("1234678910111213" , 1, 32331, "09-04-25", 123);
INSERT into Product values(null, "Alpha", "Best Air Cond");
INSERT into Contact values(null, "Israel", "Israeli", "israel@gmail.com", "0521111111", 1);
INSERT into PaymentDetails values(null, "1234678910111213", 1);
INSERT into IOTItem values("00001", 1);
INSERT into PaymentHistory values(null, 1, "14-03-20", 1, false), (null, 1, "15-03-20", 1, true);
INSERT into CompanyContacts values(null, 1);
INSERT into IOTToUserContact values(null, "00001", 1);
INSERT into IOTEvent values(null, "00001", "software update 1.1", null);
