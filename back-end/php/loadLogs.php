<?php
	// Include database parameters
	include("connection.php");

	// Read post data
	$data = json_decode(file_get_contents("php://input"));

	// Selected the correct SQL query depending if there is to select information from all users or just for a single user
	if ($data->targetRegistry == "all") $sql = "SELECT * FROM events";
	else $sql = "SELECT * FROM events WHERE target_registry = " . $data->targetRegistry;

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
			// Check if there is a result (i.e. if there was found any event)
			if ($resultsFound > 0) {				
				$i = 0;
				// Mount the return in JSON format
				echo "[";
				while($row = $result->fetch_assoc()) {
				$i = $i + 1;
				$return["id"] = $row["event_id"];
				$return["sourceId"] = $row["source_registry"];
				$return["targetId"] = $row["target_registry"];
				$return["type"] = $row["event_type"];
				$return["diffCredCellphone"] = $row["diff_cred_cellphone"];
				$return["diffCredTag"] = $row["diff_cred_tag"];
				$return["toResolve"] = $row["to_resolve"];
				$return["time"] = $row["event_time"];
				echo json_encode($return);
				if ($i != ($resultsFound)) echo ",";
				}
				echo "]";
			}
			else {
				echo "Error - No event found";
			}
		}
	}
	$conn->close(); // Close connection
?>