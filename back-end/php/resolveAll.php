<?php
	// Include database parameters
	include("connection.php");
	
	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// SQL query to update a single event to resolve all events
	$sqlResolveAll = "UPDATE events SET `to_resolve` = 0";

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$result = $conn->query($sqlResolveAll);
		// Check connection
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else echo "Update Successful!";
	}

	$conn->close(); // Close connection

?>