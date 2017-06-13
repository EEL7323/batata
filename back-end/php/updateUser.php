<?php
	include("connection.php");
	$data = json_decode(file_get_contents("php://input"));
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	$newPassword = sha1($data->newPassword);
	$oldPassword = sha1($data->oldPassword);

	// Check connection

	if ($data->accessLevel == 1) $toResolve = 1;
	else if ($data->accessLevel == 0) $toResolve = 0;

	if ($data->oldPassword == "" || $data->newPassword == "") $sqlUpdateUser = "UPDATE users SET `tag_number` = " . $data->tagNumber . ", `name` = '" . $data->name . "', `access_level` = " . $data->accessLevel . " WHERE `registry_number` = '" . $data->targetRegistryNumber . "'";
	else {
		$sqlUpdateUser = "UPDATE users SET `tag_number` = " . $data->tagNumber . ", `name` = '" . $data->name . "', `access_level` = " . $data->accessLevel . ", `password` = '" . $newPassword ."' WHERE `registry_number` = '" . $data->targetRegistryNumber . "' AND `password` = '" . $oldPassword . "'";
		$sqlCheckUserInfo = "SELECT registry_number FROM users WHERE registry_number = '" . $data->sourceRegistryNumber . "' AND `password` = '" . $oldPassword . "'";
	}
	
	$sqlUpdateUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', '$data->targetRegistryNumber', 2, 0, 0, $toResolve)";

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		if (isset($sqlCheckUserInfo)) {
			$result = $conn->query($sqlCheckUserInfo);
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else {
				$resultsFound = $result->num_rows;
				if ($resultsFound == 1) {
					$conn->query($sqlUpdateUser);
					if ($conn->error) echo "Error - Server error while updating database: Check if you typed the correct old password for your user." . $conn->error;
					else {
						$conn->query($sqlUpdateUserEvent);
						if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
						else echo "Update Successful!";
					}
				}
				else echo "Error - Server error while updating database: Check if you typed the correct old password for your user.";
			}
		}
		else {
			$conn->query($sqlUpdateUser);
			if ($conn->error) echo "Error - Server error while updating database: Check if you typed the correct old password for your user.";
			else {
				$conn->query($sqlUpdateUserEvent);
				if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
				else echo "Update Successful!";
			}
		}
	}
	$conn->close();
?>