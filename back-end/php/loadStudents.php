<?php
	// Include database parameters
	include("connection.php");

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// SQL query to select basic information from students
		$sql = "SELECT name, registry_number, cellphone_credit, card_credit FROM users WHERE access_level = 1";
		// Execute query
		$result = $conn->query($sql);
		// Check connection
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;
			// Check if there is a result (i.e. if there was found any student)
			if ($resultsFound > 0) {
				$i = 0;
				// Mount return string
				echo "[";
				while($row = $result->fetch_assoc()) {
				$return["name"] = $row["name"];
				$return["registryNumber"] = $row["registry_number"];
				$return["cellphoneCredit"] = $row["cellphone_credit"];
				$return["cardCredit"] = $row["card_credit"];
				$return["id"] = $i;
				$i = $i + 1;
				echo json_encode($return);
				if ($i != ($resultsFound)) echo ",";
				}
				echo "]";
			}
			else {
				echo "Error - No student found";
			}
		}
	}
	$conn->close(); // Close connection
?>