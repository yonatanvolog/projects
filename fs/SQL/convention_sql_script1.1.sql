DROP DATABASE tadiran;

CREATE DATABASE tadiran;
USE tadiran;

CREATE TABLE PaymentHistory (
	pk_payment_history_id int AUTO_INCREMENT PRIMARY KEY,
	fk_payment_id int NOT NULL,
    payment_date date,
    amount int NOT NULL,
    approved boolean NOT NULL
);

CREATE TABLE CompanyContacts (
    pk_company_user_id int AUTO_INCREMENT PRIMARY KEY,
    fk_contact_id int NOT NULL
);

CREATE TABLE IOTToUserContact (
    pk_id int AUTO_INCREMENT PRIMARY KEY,
    fk_serial_number varchar(16) NOT NULL,
    fk_contact_id int NOT NULL
);

CREATE TABLE IOTEvent (
    pk_iot_event_id int AUTO_INCREMENT PRIMARY KEY,
	fk_serial_number varchar(16) NOT NULL,
    description varchar(255) NOT NULL,
    time_stamp TimeStamp DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IOTItem (
    pk_serial_number varchar(16) PRIMARY KEY,
    fk_product_id int NOT NULL
);

CREATE TABLE Product (
    pk_product_id int AUTO_INCREMENT PRIMARY KEY,
    product_name varchar(20) NOT NULL UNIQUE,
    description varchar(80)
);

CREATE TABLE Contact (
    pk_contact_id int AUTO_INCREMENT PRIMARY KEY,
    first_name varchar(20) NOT NULL,
    last_name varchar(40) NOT NULL,
    email varchar(20) NOT NULL UNIQUE,
    phone varchar(20) NOT NULL UNIQUE,
    fk_address_id int
);

CREATE TABLE PaymentDetails (
    pk_payment_id int AUTO_INCREMENT PRIMARY KEY,
    fk_credit_card_number varchar(16),
    fk_billing_address int NOT NULL
);

CREATE TABLE CardDetails (
    pk_credit_card_number varchar(16) PRIMARY KEY,
    fk_cc_comapny_id int NOT NULL UNIQUE,
    credit_card_holder_id int NOT NULL,
    expire_date Date,
    security_code int
);

CREATE TABLE CreditCardCompany (
    pk_cc_company_id int AUTO_INCREMENT PRIMARY KEY,
    company_name varchar(40) UNIQUE
);

CREATE TABLE Address (
    pk_address_id int AUTO_INCREMENT PRIMARY KEY,
    zip varchar(10),
    address varchar(80) NOT NULL,
    fk_city_id int NOT NULL
);

CREATE TABLE City (
    pk_city_id int AUTO_INCREMENT PRIMARY KEY,
    city_name varchar(40) NOT NULL,
    fk_country_id int NOT NULL
);

CREATE TABLE Country (
    pk_country_id int AUTO_INCREMENT PRIMARY KEY,
    country_name varchar(20) NOT NULL UNIQUE
);

ALTER TABLE PaymentHistory 
ADD FOREIGN KEY (fk_payment_id) REFERENCES PaymentDetails(pk_payment_id);

ALTER TABLE CompanyContacts 
ADD FOREIGN KEY (fk_contact_id) REFERENCES Contact(pk_contact_id);

ALTER TABLE IOTToUserContact 
ADD FOREIGN KEY (fk_serial_number) REFERENCES IOTItem(pk_serial_number),
ADD FOREIGN KEY (fk_contact_id) REFERENCES Contact(pk_contact_id);

ALTER TABLE IOTEvent 
ADD FOREIGN KEY (fk_serial_number) REFERENCES IOTItem(pk_serial_number);

ALTER TABLE IOTItem 
ADD FOREIGN KEY (fk_product_id) REFERENCES Product(pk_product_id);

ALTER TABLE Contact 
ADD FOREIGN KEY (fk_address_id) REFERENCES Address(pk_address_id);

ALTER TABLE PaymentDetails 
ADD FOREIGN KEY (fk_credit_card_number) REFERENCES CardDetails(pk_credit_card_number),
ADD FOREIGN KEY (fk_billing_address) REFERENCES Address(pk_address_id);

ALTER TABLE CardDetails 
ADD FOREIGN KEY (fk_cc_comapny_id) REFERENCES CreditCardCompany(pk_cc_company_id);

ALTER TABLE Address
ADD FOREIGN KEY (fk_city_id) REFERENCES City(pk_city_id);

ALTER TABLE City 
ADD FOREIGN KEY (fk_country_id) REFERENCES Country(pk_country_id);

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
