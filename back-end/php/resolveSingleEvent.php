<?php
	include("connection.php");
	
	$eventId = file_get_contents("php://input");
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	$sqlResolveAll = "UPDATE events SET `to_resolve` = 0 WHERE `event_id` = " . $eventId;

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		$result = $conn->query($sqlResolveAll);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else echo "Update Successful!";
	}

	$conn->close();

?>