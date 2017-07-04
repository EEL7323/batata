<?php
	// Include database parameters
	include("connection.php");

	// Read post data
	$data = json_decode(file_get_contents("php://input"));
	
	// Select the correct SQL query depending if there is to select information from all users or just for a single user
	if ($data->registryNumber == "all") $sql = "SELECT name, registry_number, tag_number, cellphone_credit, card_credit, access_level FROM users";
	else $sql = "SELECT name, registry_number, tag_number, cellphone_credit, card_credit, access_level FROM users WHERE `registry_number` = " . $data->registryNumber;
	
	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$result = $conn->query($sql);
		// Check connection
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;
			// Check if there is a result (i.e. if there was found any user)
			if ($resultsFound > 0) {
				$i = 0;
				// Mount return string
				echo "[";
				while($row = $result->fetch_assoc()) {
				$return["name"] = $row["name"];
				$return["registryNumber"] = $row["registry_number"];
				$return["tagNumber"] = $row["tag_number"];
				$return["cellphoneCredit"] = $row["cellphone_credit"];
				$return["cardCredit"] = $row["card_credit"];
				$return["accessLevel"] = $row["access_level"];
				$return["id"] = $i;
				$i = $i + 1;
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