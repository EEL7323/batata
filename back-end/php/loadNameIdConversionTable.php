<?php
	// Include database parameters
	include("connection.php");

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// SQL query to select all registry number and tag number for a conversion
		$sql = "SELECT tag_number, registry_number FROM users";
		// Execute query
		$result = $conn->query($sql);
		// Check connection
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;
			// Check if there is a result (i.e. if there was found any user)
			if ($resultsFound > 0) {				
				$i = 0;
				echo "[";
				// Mount return in JSON format
				while($row = $result->fetch_array()) {
				$i = $i + 1;
				$return["tagNumber"] = $row["tag_number"];
				$return["registryNumber"] = $row["registry_number"];
				echo json_encode($return);
				if ($i != ($resultsFound)) echo ",";
				}
				echo "]";
			}
			else {
				echo "Error - No user found";
			}
		}
	}
	$conn->close(); // Close connection
?>