<?php
	
	// Include database parameters
	include("connection.php");
	
	// Read post data
	$data = json_decode(file_get_contents("php://input"));
	
	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// SQL query used to insert an event of adding credits
	$sqlUpdateCreditsEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', '$data->targetRegistryNumber', 5, $data->diffCreditCellphone, $data->diffCreditTag, 1)";

	// Only the board can change the credits in 'users' table
	// The event is created and when board resolves the pending tasks it will update the credits

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$conn->query($sqlUpdateCreditsEvent);
		// Check connection
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else echo "Update Successful! The credits will be updated as soon as the board syncs.";	
	}
	$conn->close(); // Close connection
?>