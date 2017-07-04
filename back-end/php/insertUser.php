<?php
	// Include database parameters
	include("connection.php");

	// Read post data
	$data = json_decode(file_get_contents("php://input"));
	
	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Select the correct value to toResolve variable depending if the user is an admin or a student
	if ($data->accessLevel == 1) $toResolve = 1;
	else if ($data->accessLevel == 0) $toResolve = 0;

	// SQL query used to insert a new user with the parameters passed in the post request
	$sqlAddUser = "INSERT INTO users (`tag_number`, `registry_number`, `name`, `access_level`, `password`, `cellphone_credit`, `card_credit`, `web_token`) VALUES ($data->tagNumber, '$data->targetRegistryNumber', '$data->name', $data->accessLevel, 'c6bcef53d57d8a756d50e36f2ea87a5de4522859', 0, 0, 'logged-out')";
	
	// SQL query used to insert an event of the creation of a new user
	$sqlAddUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', '$data->targetRegistryNumber', 0, 0, 0, $toResolve)";

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {		
		// Execute query
		$conn->query($sqlAddUser);
		// Check connection
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			// Execute query
			$conn->query($sqlAddUserEvent);
			// Check connection
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else echo "Insertion Successful!";
		}		
	}
	$conn->close(); // Close connection
?>