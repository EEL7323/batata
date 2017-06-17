<?php
	include("connection.php");

	$data = file_get_contents("php://input");

	$strings = explode(",",$data);
	$tagNumber = $strings[0];
	$eventCode = $strings[1];

	if ($eventCode == 3) $toResolve = 1;
	else $toResolve = 0;
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	$sqlGetRegistry = "SELECT registry_number FROM users WHERE `tag_number` =" . $tagNumber;

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
			}
			else $registryNumber = "not found";
			$sqlInsertEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('board', '$registryNumber', $eventCode, 0, 0, $toResolve)";
			$conn->query($sqlInsertEvent);
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else {
				echo "Update Successful!";
			}
		}
	}

	$conn->close();

?>