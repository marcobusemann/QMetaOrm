create table address (
	id int not null primary key,
	country varchar(30) not null,
	postcode varchar(30) not null,
	street varchar(30) not null
);

create sequence address_gen;

create table person (
	id int not null primary key,
	surname varchar(30) not null,
	name varchar(30) not null,
	address int references address (id)
);

create sequence person_gen;