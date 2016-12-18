create table address (
   id int not null primary key,
   country varchar(30) not null,
   postCode varchar(30) not null,
   street varchar(30) not null
);

create table person (
   id int not null primary key,
   name varchar(30) not null,
   surname varchar(30),
   address references address(id)
);

