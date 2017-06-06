<?php

// Include connection file, in which could be changed database settings
include("connection.php");

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

	// Admin user insertion query
$sqlAddAdminUser = "INSERT INTO users (`tag_number`, `registry_number`, `name`, `access_level`, `password`, `cellphone_credit`, `card_credit`, `web_token`) VALUES (0, '00000000', 'Admin', 0, 'c73f7357c0a53dc07978cee4b77a3c322c7699e0', 0, 0, 'invalid')";
$sqlAddUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES (0, 0, 0, 0, 0, 0)";

// Execution and tests

if ($conn->query($sqlCreateDatabase) === TRUE) {

	$conn->close();
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	if ($conn->connect_error) {
		echo("Connection failed: " . $conn->connect_error);
	} 
	else {
		$creationResult = $conn->query($sqlCreateUsersTable) and $conn->query($sqlCreateEventsTable);
		
		if ($creationResult) {
			$insertionResult = $conn->query($sqlAddAdminUser) and $conn->query($sqlAddUserEvent);
			if ($insertionResult) echo("Insertion Successful"); 
			else echo("Insertion Error: " . $conn->error);
		}
		else echo("Configuration Error: $conn->error");
	}
}

$conn->close();

?>