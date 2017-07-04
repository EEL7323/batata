<?php
	// Include database parameters
	include("connection.php");

	// Read post data
	$data = json_decode(file_get_contents("php://input"));
	
	// Convert the password to sha1 (hash function)
	$newPassword = sha1($data->newPassword);
	$oldPassword = sha1($data->oldPassword);

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Select the correct SQL query depending if the password is going to be changed or not
	if ($data->oldPassword == "" || $data->newPassword == "") $sqlUpdateUser = "UPDATE users SET `tag_number` = " . $data->tagNumber . ", `name` = '" . $data->name . "', `access_level` = " . $data->accessLevel . " WHERE `registry_number` = '" . $data->targetRegistryNumber . "'";
	else {
		$sqlUpdateUser = "UPDATE users SET `name` = '" . $data->name . "', `password` = '" . $newPassword ."' WHERE `registry_number` = '" . $data->targetRegistryNumber . "' AND `password` = '" . $oldPassword . "'";
		$sqlCheckUserInfo = "SELECT registry_number FROM users WHERE registry_number = '" . $data->sourceRegistryNumber . "' AND `password` = '" . $oldPassword . "'";
	}
	
	// SQL query to insert the user change event
	$sqlUpdateUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', '$data->targetRegistryNumber', 2, 0, 0, 0)";
	
	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		if (isset($sqlCheckUserInfo)) {
			// Execute query
			$result = $conn->query($sqlCheckUserInfo);
			// Check connection
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else {
				$resultsFound = $result->num_rows;
				// Check if there is one and only one user for that registry number and password combination
				if ($resultsFound == 1) {
					// Execute query
					$conn->query($sqlUpdateUser);
					// Check connection
					if ($conn->error) echo "Error - Server error while updating database: Check if you typed the correct old password for your user." . $conn->error;
					else {
						// Execute query
						$conn->query($sqlUpdateUserEvent);
						// Check connection
						if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
						else echo "Update Successful!";
					}
				}
				else echo "Error - Server error while updating database: Check if you typed the correct old password for your user.";
			}
		}
		else {
			// Execute query
			$conn->query($sqlUpdateUser);
			// Check connection
			if ($conn->error) echo "Error - Server error while updating database: Check if you typed the correct old password for your user.";
			else {
				// Execute query
				$conn->query($sqlUpdateUserEvent);
				// Check connection
				if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
				else echo "Update Successful!";
			}
		}
	}
	$conn->close(); // Close connection
?>