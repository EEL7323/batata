<?php

// Include connection file, in which could be changed database settings
include("connection.php");

// Queries
	// Database creation query
$sqlCreateDatabase = "CREATE DATABASE IF NOT EXISTS " . $dbName;

	// Table "user_records" creation query
$sqlCreateUserRecordsTable = "CREATE TABLE IF NOT EXISTS user_records (
  tag_number INT UNSIGNED PRIMARY KEY,
  registry_number VARCHAR(10),
  name VARCHAR(40) NOT NULL,
  access_level BIT,
  password VARCHAR(40) NOT NULL
  )";
 
	// Table "user_status" creation query
$sqlCreateUserStatusTable = "CREATE TABLE IF NOT EXISTS user_status (
  tag_number INT UNSIGNED PRIMARY KEY,
  cellphone_credit SMALLINT,
  card_credit SMALLINT,
  cellphone_sync BIT,
  board_sync BIT,
  web_token VARCHAR(60) NOT NULL
  )";

	// Table "user_events" creation query
$sqlCreateUserEventsTable = "CREATE TABLE IF NOT EXISTS user_events (
  tag_number INT UNSIGNED PRIMARY KEY,
  event_type TINYINT UNSIGNED,
  diff_cred_cellphone SMALLINT,
  diff_cred_tag SMALLINT,
  event_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  )";

	// Admin user insertion query
$sqlAddAdminUserToRecords = "INSERT INTO user_records (`tag_number`, `registry_number`, `name`, `access_level`, `password`) VALUES (0, '00000000', 'Admin', 0, 'c73f7357c0a53dc07978cee4b77a3c322c7699e0')";
$sqlAddAdminUserToStatus = "INSERT INTO user_status (`tag_number`, `cellphone_credit`, `card_credit`, `cellphone_sync`, `board_sync`, `web_token`) VALUES (0, 0, 0, 0, 0, 'logged-out')";
$sqlAddAdminUserToEvents = "INSERT INTO user_events (`tag_number`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`) VALUES (0, 0, 0, 0)";

// Execution and tests

if ($conn->query($sqlCreateDatabase) === TRUE) {

	$conn->close();
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	if ($conn->connect_error) {
		echo("Connection failed: " . $conn->connect_error);
	} 
	else {
		$creationResult = $conn->query($sqlCreateUserRecordsTable) and	$conn->query($sqlCreateUserStatusTable) and $conn->query($sqlCreateUserEventsTable);
		
		if ($creationResult) {
			$insertionResult = $conn->query($sqlAddAdminUserToRecords) and $conn->query($sqlAddAdminUserToStatus) and $conn->query($sqlAddAdminUserToEvents);
			if ($insertionResult) echo("Insertion Successful"); 
			else echo("Insertion Error: " . $conn->error);
		}
		else echo("Configuration Error: $conn->error");
	}
}

$conn->close();

?>