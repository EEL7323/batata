<?php
	include("connection.php");

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		$sql = "SELECT name, registry_number, tag_number FROM user_records WHERE access_level != 0";
		$result = $conn->query($sql);
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->connect_error;
		else {
			$resultsFound = $result->num_rows;
			if ($resultsFound > 0) {
				$result = $result->fetch_assoc();
				echo json_encode($result);
			}
			else {
				echo "Error - No user found";
			}
		}
	}

?>