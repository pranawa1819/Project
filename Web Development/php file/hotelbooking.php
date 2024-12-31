<?php
// Database credentials
$host = "localhost";
$username = "root"; 
$password = "";     
$database = "hotel_booking";

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
$sql = "CREATE TABLE IF NOT EXISTS guest_info ( 
    id INT AUTO_INCREMENT PRIMARY KEY,
    checkin DATE NOT NULL,
    checkout DATE NOT NULL,
    numberofguest VARCHAR(50) NOT NULL
)"; 

if ($conn->query($sql) === TRUE) { 
    echo "<br>**TABLE IS CREATED OR ALREADY EXISTS!**"; 
} else { 
    die("TABLE CREATION FAILED!!<br>" . $conn->error); 
}

// Check if the form is submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve form data
    $checkin = $_POST['check-in'];
    $checkout = $_POST['check-out'];
    $numberofguest = $_POST['guests'];

    // Insert data into the database using prepared statement
    $stmt = $conn->prepare("INSERT INTO guest_info (checkin, checkout, numberofguest) VALUES (?, ?, ?)");
    $stmt->bind_param("sss", $checkin, $checkout, $numberofguest);

    if ($stmt->execute()) {
        echo "<script>alert('Booking successfully!'); window.location.href = 'hotel.html';</script>";
    } else {
        echo "Error: " . $stmt->error;
    }

    $stmt->close();
    $conn->close();
}
?>
