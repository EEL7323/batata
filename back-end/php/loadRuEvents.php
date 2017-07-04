<?php
	// Include database parameters
	include("connection.php");
	
	// Include function to get 2D array of users index
	include("get2DArrayIndex.php");

	// SQL query to load the conversion table between tag number and registry number
	$sqlLoadConversionTable = "SELECT tag_number, registry_number FROM users WHERE `access_level` = 1";

	// SQL query to load all events with flag to be resolved by the board
	$sqlLoadEvents = "SELECT * FROM events WHERE `to_resolve` = 1";

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$result = $conn->query($sqlLoadConversionTable);
		// Check connection
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$array = array();
			$i = 0;
			// Mount array with the results
			while($row = $result->fetch_assoc()) {
			$array[$i][0] = $row["registry_number"];
			$array[$i][1] = $row["tag_number"];
			$i = $i + 1;
			}
			// Execute query
			$result = $conn->query($sqlLoadEvents);
			// Check connection
			if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
			else {
				$resultsFound = $result->num_rows;
				// Check if there is a result (i.e. if there was found any event)
				if ($resultsFound > 0) {				
					$i = 0;
					// Mount result in JSON format
					echo "[";
					while($row = $result->fetch_assoc()) {
						$index = getIndexOfRegistry($row["target_registry"],$array);
						if ($index == -1) {
							$registryNumber = $row["target_registry"];
							$sqlGetTagNumber = "SELECT tag_number FROM users_buff WHERE registry_number = '" . $registryNumber . "'"; 
							$resultTag = $conn->query($sqlGetTagNumber);
							if ($conn->error) {
								echo "Error - Server error while consulting database: " . $conn->error;
								$tagNumber = "Not found";
							}
							else {
								$tagsFound = $resultTag->num_rows;
								if ($tagsFound == 1) {
									$resultTag = $resultTag->fetch_assoc();
									$tagNumber = $resultTag["tag_number"];
								}
								else $tagNumber = "Not found";		
							}									
						}
						else $tagNumber = $array[$index][1];
						$return["eventId"] = $row["event_id"];
						$return["tagNumber"] = $tagNumber;
						$return["type"] = $row["event_type"];
						$return["diffCredCellphone"] = $row["diff_cred_cellphone"];
						$return["diffCredTag"] = $row["diff_cred_tag"];
						echo json_encode($return);
						$i = $i + 1;
						if ($i != ($resultsFound)) echo ",";
					}
					echo "]";
				}
				else "Error - No event found";
			}
		}
	}
	$conn->close(); // Close connection
?>