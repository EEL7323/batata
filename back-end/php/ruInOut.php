<?php
	include("connection.php");

	$data = file_get_contents("php://input");
	
	$strings = explode(",",$data);
	$tagNumber = $strings[0];
	$eventCode = $strings[1];
	$sourceCode = $strings[2];
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	$sqlGetRegistry = "SELECT registry_number, cellphone_credit, card_credit FROM users WHERE `tag_number` =" . $tagNumber;

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		$result = $conn->query($sqlGetRegistry);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;
			if ($resultsFound == 1) {
				$result = $result->fetch_assoc();
				$registryNumber = $result["registry_number"];
				$cellphoneCredit = $result["cellphone_credit"] - 1;
				$cardCredit = $result["card_credit"] - 1;
				$deltaCard = 0;
				$deltaCellphone = 0;
				if ($eventCode == 3) {
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
						$conn->query($sqlUpdateCredits);
						if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
				}	
				$sqlInsertEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('board', '$registryNumber', $eventCode, $deltaCellphone, $deltaCard, 0)";
				$conn->query($sqlInsertEvent);
				if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
				else echo "Update Successful!";
			}
			else echo "Error - User not found.";
		}
	}

	$conn->close();

?>