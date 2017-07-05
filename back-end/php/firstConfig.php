<?php

// Include connection file, in which could be changed database settings
include("connection.php");

// Create first connection
$conn = new mysqli($serverName, $userName, $password);

// Check connection
if ($conn->connect_error) {
    echo ("Connection failed: " . $conn->connect_error);
} 

// Queries
	// Database creation query
$sqlCreateDatabase = "CREATE DATABASE IF NOT EXISTS " . $dbName;

	// Table "users" creation query
$sqlCreateUsersTable = "CREATE TABLE IF NOT EXISTS users (
  tag_number INT UNSIGNED PRIMARY KEY,
  registry_number VARCHAR(10),
  name VARCHAR(40) NOT NULL,
  access_level BIT,
  password VARCHAR(40) NOT NULL,
  cellphone_credit SMALLINT,
  card_credit SMALLINT,
  web_token VARCHAR(60) NOT NULL
  )";

	// Table "events" creation query
$sqlCreateEventsTable = "CREATE TABLE IF NOT EXISTS events (
  event_id INT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
  source_registry VARCHAR(10),
  target_registry VARCHAR(10),
  event_type TINYINT UNSIGNED,
  diff_cred_cellphone SMALLINT,
  diff_cred_tag SMALLINT,
  to_resolve BIT,
  event_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  )";
  
  // Table "users_buff" creation query
 $sqlCreateUsersBuffTable = "CREATE TABLE IF NOT EXISTS users_buff (
  tag_number INT UNSIGNED PRIMARY KEY,
  registry_number VARCHAR(10)
  )";

	// Admin user insertion query and event query
$sqlAddAdminUser = "INSERT INTO users (`tag_number`, `registry_number`, `name`, `access_level`, `password`, `cellphone_credit`, `card_credit`, `web_token`) VALUES (0, '00000000', 'Admin', 0, 'c6bcef53d57d8a756d50e36f2ea87a5de4522859', 0, 0, '')";
$sqlAddUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES (0, '00000000', 0, 0, 0, 0)";

// Execute query to create the databasae
if ($conn->query($sqlCreateDatabase) === TRUE) {

	$conn->close(); // Close first connection
	
	// Open connection using the database batata
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Test connection
	if ($conn->connect_error) {
		echo("Connection failed: " . $conn->connect_error);
	} 
	else {
		// Execute queries
		$creationResult = $conn->query($sqlCreateUsersTable) and $conn->query($sqlCreateEventsTable) and $sqlCreateUsersBuffTable;
		// Check connection
		if ($creationResult) {
			// Execute queries
			$insertionResult = $conn->query($sqlAddAdminUser) and $conn->query($sqlAddUserEvent) and $conn->query($sqlCreateUsersBuffTable);
			// Check connection
			if ($insertionResult) echo("Insertion Successful"); 
			else echo("Insertion Error: " . $conn->error);
		}
		else echo("Configuration Error: " . $conn->error);
	}
}

$conn->close(); // Close connection

?>