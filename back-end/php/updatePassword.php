<?php
	include("connection.php");
	$data = json_decode(file_get_contents("php://input"));
	
	$newPassword = sha1($data->newPassword);
	$oldPassword = sha1($data->oldPassword);

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	$sqlCheckUserInfo = "SELECT registry_number FROM users WHERE registry_number = '" . $data->sourceRegistryNumber . "' AND `password` = '" . $oldPassword . "'";
	$sqlUpdateUser = "UPDATE users SET `password` = '" . $newPassword . "' WHERE `registry_number` = '" . $data->sourceRegistryNumber . "' AND `password` = '" . $oldPassword . "'";
	$sqlUpdateUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', '$data->sourceRegistryNumber', 2, 0, 0, 0)";

	// Check connection

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		$result = $conn->query($sqlCheckUserInfo);
		if ($conn->error) echo "Error - Server error while updating database: check if you typed the correct old password for your user.";
		else {
			$resultsFound = $result->num_rows;
			if ($resultsFound == 1) {
				$conn->query($sqlUpdateUser);
				if ($conn->error) echo "Error - Server error while updating database: Check if you typed the correct old password for your user.";
				else {
					$conn->query($sqlUpdateUserEvent);
					if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
					else echo "Update Successful!";
				}
			}
			else echo "Error - Server error while updating database: Check if you typed the correct old password for your user.";
		}
	}
	$conn->close();
?>