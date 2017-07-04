<?php
	// Include database parameters
	include("connection.php");

	// Read post data
	$data = file_get_contents("php://input");
	
	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Get parts of the input string (board can only send strings - not in JSON format, data separeted by commas)
	$strings = explode(",",$data);
	$tagNumber = $strings[0];
	$cellphoneCredit = $strings[1];
	$cardCredit = $strings[2];
	$eventId = $strings[3];
	
	// SQL query to update user credits in the users table
	$sqlUpdateUserCredits = "UPDATE users SET `cellphone_credit` = " . $cellphoneCredit . ", `card_credit` = " . $cardCredit . " WHERE `tag_number` = " . $tagNumber;
	
	// SQL query to update the credit event to resolved
	$sqlUpdateCreditsEvent = "UPDATE events SET `to_resolve` = 0 WHERE `event_id` = ". $eventId;

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$conn->query($sqlUpdateUserCredits);
		// Check connection
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			// Execute query
			$conn->query($sqlUpdateCreditsEvent);
			// Check connection
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else echo "Update Successful!";	
		}
	}
	$conn->close(); // Close connection
?>