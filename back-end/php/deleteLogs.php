<?php
	include("connection.php");

	$data = json_decode(file_get_contents("php://input"));

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {

		$sqlDeleteLogs = "DELETE FROM events WHERE event_type <> 6 AND to_resolve <> 1";
		$sqlInsertClearLogEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', 'server', 6, 0, 0, 0)";
		
		$conn->query($sqlDeleteLogs);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			$conn->query($sqlInsertClearLogEvent);
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else echo "Delete Successful!";
		}
	}
	$conn->close();
?>