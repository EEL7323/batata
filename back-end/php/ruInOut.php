<?php
	// Include database parameters
	include("connection.php");

	// Read post data
	$data = file_get_contents("php://input");
	
	// Get parts of the input string (board can only send strings - not in JSON format, data separeted by commas)
	$strings = explode(",",$data);
	$tagNumber = $strings[0];
	$eventCode = $strings[1];
	$sourceCode = $strings[2];
	
	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// SQL query to load the information of the user entering/leaving RU
	$sqlGetRegistry = "SELECT registry_number, cellphone_credit, card_credit FROM users WHERE `tag_number` =" . $tagNumber;

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$result = $conn->query($sqlGetRegistry);
		// Check connection
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;
			// Check if there is one and only one user for that tagNumber
			if ($resultsFound == 1) {
				$result = $result->fetch_assoc();
				$registryNumber = $result["registry_number"];
				$cellphoneCredit = $result["cellphone_credit"] - 1;
				$cardCredit = $result["card_credit"] - 1;
				$deltaCard = 0;
				$deltaCellphone = 0;
				// If it is an entering event, the credits are discounted at the same time
				if ($eventCode == 3) {
						// Source code indicates if the credits should be discounted of cellphone or the id card
						if ($sourceCode == 1) {
							$sqlUpdateCredits = "UPDATE users SET `cellphone_credit` = " . $cellphoneCredit . " WHERE `registry_number` = '" . $registryNumber . "'";
							$deltaCard = 0;
							$deltaCellphone = -1;
						}
						else {
							$sqlUpdateCredits = "UPDATE users SET `card_credit` = " . $cardCredit . " WHERE registry_number = '" . $registryNumber . "'";
							$deltaCard = -1;
							$deltaCellphone = 0;
						}
						// Execute query
						$conn->query($sqlUpdateCredits);
						// Check connection
						if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
				}
				// SQL query used to insert an event of entering or leaving RU
				$sqlInsertEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('board', '$registryNumber', $eventCode, $deltaCellphone, $deltaCard, 0)";
				// Execute query
				$conn->query($sqlInsertEvent);
				// Check connection
				if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
				else echo "Update Successful!";
			}
			else echo "Error - User not found.";
		}
	}

	$conn->close(); // Close connection

?>