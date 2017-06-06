<?php
	include("connection.php");
	$data = json_decode(file_get_contents("php://input"));
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	$sqlUpdateCreditsEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', '$data->targetRegistryNumber', 5, $data->diffCreditCellphone, $data->diffCreditTag, 1)";

	// Only the board can change the credits in 'users' table
	// The event is created and when board resolves the pending tasks it will update the credits

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {		
		$conn->query($sqlUpdateCreditsEvent);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->connect_error;
		else echo "Update Successful! Waiting for board to sync...";	
	}

?>