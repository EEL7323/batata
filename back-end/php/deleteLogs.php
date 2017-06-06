<?php
	include("connection.php");

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {

		$sql = "DELETE FROM events";
		
		$conn->query($sql);
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->connect_error;
		else {
			echo "Delete Successful!";
		}

	}

?>