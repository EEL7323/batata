<?php
	// Include database parameters
	include("connection.php");
	
	// Read post data
	$data = json_decode(file_get_contents("php://input"));

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// SQL query used to delete all logs which are not to resolve and are not of clear_logs type
		$sqlDeleteLogs = "DELETE FROM events WHERE event_type <> 6 AND to_resolve <> 1";
		// SQL query used to insert the event of clear logs
		$sqlInsertClearLogEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$data->sourceRegistryNumber', 'server', 6, 0, 0, 0)";
		// Execute query
		$conn->query($sqlDeleteLogs);
		// Check connection
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			// Execute query
			$conn->query($sqlInsertClearLogEvent);
			// Check connection
			if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
			else echo "Delete Successful!";
		}
	}
	$conn->close(); // Close connection
?>