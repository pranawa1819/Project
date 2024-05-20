-- Create the database
CREATE DATABASE hotel_management;
USE hotel_management;

-- Create the "rooms" table
CREATE TABLE rooms (
    room_number INT PRIMARY KEY,
    room_type VARCHAR(20),
    room_price DOUBLE,
    guest_name VARCHAR(50),
    phone_number VARCHAR(20),
    check_in_date DATE
);
select *from rooms;
drop table rooms;
-- Create the "checkouts" table
CREATE TABLE checkouts (
    room_number INT PRIMARY KEY,
    guest_name VARCHAR(50),
    check_in_date DATE,
    check_out_date DATE,
    room_type VARCHAR(20)
);
select *from checkouts;
drop table checkouts;
