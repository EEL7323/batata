<?php
	// Include database parameters
	include("connection.php");
	
	// Read post data
	$eventId = file_get_contents("php://input");
	
	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// SQL query to update a single event to resolved
	$sqlResolve = "UPDATE events SET `to_resolve` = 0 WHERE `event_id` = " . $eventId;

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$result = $conn->query($sqlResolve);
		// Check connection
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else echo "Update Successful!";
	}

	$conn->close(); // Close connection

?>