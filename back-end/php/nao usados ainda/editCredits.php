<?php
	include("connection.php");

	$data = json_decode(file_get_contents("php://input"));

	$userRegistry = $data->registry;
	$cellCreditIncrement = $data->cellCreditIncrement;
	$cardCreditIncrement = $data->cardCreditIncrement;

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	$sql = "SELECT tag_number FROM user_records WHERE registry_number='$userRegistry'";

	// Try to connect
	if ($conn->connect_error) echo("Error - Connection failed: " . $conn->connect_error);
	else {
		// Read results
		$result = $conn->query($sql);
		$resultsFound = $result->num_rows;
		if ($resultsFound == 1) {
			$result = $result->fetch_assoc();
			$tagNumber = $result["tag_number"];

			$sql = "SELECT cellphone_credit, card_credit FROM user_status WHERE tag_number='$tagNumber'";

			$result = $conn->query($sql);
			$resultsFound = $result->num_rows;
			if ($resultsFound == 1) {
				$result = $result->fetch_assoc();
				$newCellCredit = $result["cellphone_credit"] + $cellCreditIncrement;
				$newCardCredit = $result["card_credit"] + $cellCreditIncrement;
				$sql = "UPDATE user_status SET cellphone_credit='$newCellCredit', card_credit='$newCardCredit' WHERE tag_number='$tagNumber'";
				$conn->query($sql);
				if ($conn->error) echo "Error - Server error while updating user";
				else echo "Update successful";
			}
			else {
				echo "Error - User not found";
			}
		}
	}
		$conn->close();	
?>