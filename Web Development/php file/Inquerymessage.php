<?php
// Database credentials
$host = "localhost";
$username = "root"; 
$password = "";     
$database = "travel_inquery";

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

// Create the querymessage table if it doesn't exist 
$sql = "CREATE TABLE IF NOT EXISTS querymessage ( 
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL,
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
    $message = $_POST['message'];



    // Insert data into the database
    $stmt = $conn->prepare("INSERT INTO querymessage (name, email, message) VALUES (?, ?, ?)");
    $stmt->bind_param("sss", $name, $email, $message);

    if ($stmt->execute()) {
        echo "<script>alert('Message sent successfully!'); window.location.href = 'contact.html';</script>";
    } else {
        echo "Error: " . $stmt->error;
    }

    // Close the statement and connection
    $stmt->close();
    $conn->close();
}
?>
