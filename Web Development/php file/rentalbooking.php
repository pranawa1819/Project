<?php
// Database credentials
$host = "localhost";
$username = "root"; 
$password = "";     
$database = "vechile_booking";

// Establish database connection
$conn = new mysqli($host, $username, $password);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Create the database if it doesn't exist 
$sql = "CREATE DATABASE IF NOT EXISTS $database"; 
if ($conn->query($sql) === TRUE) { 
    echo "<br>**DATABASE IS CREATED OR ALREADY EXISTS!**"; 
} else {
    die("DATABASE CREATION FAILED!!<br>" . $conn->error); 
}

// Select the database 
$conn->select_db($database); 

// Create the bookings table if it doesn't exist 
$sql = "CREATE TABLE IF NOT EXISTS Vechile_info( 
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL,
    date DATE NOT NULL,
    vechile VARCHAR(50) NOT NULL,
    message TEXT
)"; 

if ($conn->query($sql) === TRUE) { 
    echo "<br>**TABLE IS CREATED OR ALREADY EXISTS!**"; 
} else { 
    die("TABLE CREATION FAILED!!<br>" . $conn->error); 
}

// Check if the form is submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve form data
    $name = $_POST['name'];
    $email = $_POST['email'];
    $date = $_POST['Date'];
    $vechile = $_POST['vechile'];
    $message = $_POST['message'];

    // Insert data into the database using prepared statement
    $stmt = $conn->prepare("INSERT INTO Vechile_info(name, email, date, vechile, message) VALUES (?, ?, ?, ?, ?)");
    $stmt->bind_param("sssss", $name, $email, $date, $vechile, $message);
    if ($stmt->execute()) {
        echo "<script>alert('Booking successfully!'); window.location.href = 'rentalbooking.html';</script>";
    } else {
        echo "Error: " . $stmt->error;
    }
    $stmt->close();
    $conn->close();
}
?>