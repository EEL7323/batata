<?php
	include("connection.php");
	$data = json_decode(file_get_contents("php://input"));
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	if ($data->accessLevel == 1) $toResolve = 1;
	else if ($data->accessLevel == 0) $toResolve = 0;

	$sqlUpdateUser = "UPDATE users SET `tag_number` = " . $data->tagNumber . ", `name` = '" . $data->name . "', `access_level` = " . $data->accessLevel . " WHERE `registry_number` = '" . $data->targetRegistryNumber . "'";
	$sqlUpdateUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', '$data->targetRegistryNumber', 0, 0, 0, $toResolve)";

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {		
		$conn->query($sqlUpdateUser);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->connect_error;
		else {
			$conn->query($sqlUpdateUserEvent);
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->connect_error;
			else echo "Insertion Successful!";
		}		
	}

?>