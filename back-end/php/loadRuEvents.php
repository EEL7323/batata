<?php
	include("connection.php");
	include("get2DArrayIndex.php");

	$sqlLoadConversionTable = "SELECT tag_number, registry_number FROM users WHERE `access_level` = 1";
	$sqlLoadEvents = "SELECT * FROM events WHERE `to_resolve` = 1";

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		$result = $conn->query($sqlLoadConversionTable);
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$array = array();
			$i = 0;
			while($row = $result->fetch_assoc()) {
			$array[$i][0] = $row["registry_number"];
			$array[$i][1] = $row["tag_number"];
			$i = $i + 1;
			}
			$result = $conn->query($sqlLoadEvents);
			if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
			else {
				$resultsFound = $result->num_rows;
				if ($resultsFound > 0) {				
					$i = 0;
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
	$conn->close();
?>