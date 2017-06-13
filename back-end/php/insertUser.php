<?php
	include("connection.php");
	$data = json_decode(file_get_contents("php://input"));
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	if ($data->accessLevel == 1) $toResolve = 1;
	else if ($data->accessLevel == 0) $toResolve = 0;

	$sqlAddUser = "INSERT INTO users (`tag_number`, `registry_number`, `name`, `access_level`, `password`, `cellphone_credit`, `card_credit`, `web_token`) VALUES ($data->tagNumber, '$data->targetRegistryNumber', '$data->name', $data->accessLevel, 'c6bcef53d57d8a756d50e36f2ea87a5de4522859', 0, 0, 'logged-out')";
	$sqlAddUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', '$data->targetRegistryNumber', 0, 0, 0, $toResolve)";

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {		
		$conn->query($sqlAddUser);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			$conn->query($sqlAddUserEvent);
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else echo "Insertion Successful!";
		}		
	}
	$conn->close();
?>