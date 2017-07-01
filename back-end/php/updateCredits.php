<?php
	include("connection.php");
	$data = file_get_contents("php://input");
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	$strings = explode(",",$data);
	$tagNumber = $strings[0];
	$cellphoneCredit = $strings[1];
	$cardCredit = $strings[2];
	$eventId = $strings[3];
	
	$sqlUpdateUserCredits = "UPDATE users SET `cellphone_credit` = " . $cellphoneCredit . ", `card_credit` = " . $cardCredit . " WHERE `tag_number` = " . $tagNumber;
	$sqlUpdateCreditsEvent = "UPDATE events SET `to_resolve` = 0 WHERE `event_id` = ". $eventId;

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {		
		$conn->query($sqlUpdateUserCredits);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			$conn->query($sqlUpdateCreditsEvent);
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else echo "Update Successful!";	
		}
	}
	$conn->close();
?>