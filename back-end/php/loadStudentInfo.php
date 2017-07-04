<?php
	// Include database parameters
	include("connection.php");

	// Read post data
	$data = json_decode(file_get_contents("php://input"));

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Set default timezone
	date_default_timezone_set('Brazil/East');

	// Today's RU times
	$todayLunchBeginning = date('Y-m-d H:i:s', mktime(11, 0, 0, date("m"), date("d"), date("Y")));
	$todayLunchEnd = date('Y-m-d H:i:s', mktime(14, 0, 0, date("m"), date("d"), date("Y"))); // 13:30 + 30 minutes ~estimate
	$todayDinnerBeginning = date('Y-m-d H:i:s', mktime(17, 0, 0, date("m"), date("d"), date("Y")));
	$todayDinnerEnd = date('Y-m-d H:i:s', mktime(19, 30, 0, date("m"), date("d"), date("Y"))); // 19:00 + 30 minutes ~estimate

	// Yesterday's RU times
	$yesterdayLunchBeginning = date('Y-m-d H:i:s', mktime(11, 0, 0, date("m"), date("d")-1, date("Y")));
	$yesterdayLunchEnd = date('Y-m-d H:i:s', mktime(14, 0, 0, date("m"), date("d")-1, date("Y"))); // 13:30 + 30 minutes ~estimate
	$yesterdayDinnerBeginning = date('Y-m-d H:i:s', mktime(17, 0, 0, date("m"), date("d")-1, date("Y")));
	$yesterdayDinnerEnd = date('Y-m-d H:i:s', mktime(19, 30, 0, date("m"), date("d")-1, date("Y"))); // 19:00 + 30 minutes ~estimate

	// SQL Queries to calculate number of people inside RU (enter events - leave events)
	if (date('H:i:s') > date('H:i:s', mktime(15, 0, 0))) {
		$sqlGetCurrentNumberOfEnteringEventsRegistered = "SELECT COUNT(*) FROM events WHERE event_time >= '$todayDinnerBeginning' AND event_time <= '$todayDinnerEnd' AND event_type = 3";
		$sqlGetCurrentNumberOfLeavingEventsRegistered = "SELECT COUNT(*) FROM events WHERE event_time >= '$todayDinnerBeginning' AND event_time <= '$todayDinnerEnd' AND event_type = 4";
	}
	else {
		$sqlGetCurrentNumberOfEnteringEventsRegistered = "SELECT COUNT(*) FROM events WHERE event_time >= '$yesterdayLunchBeginning' AND event_time <= '$yesterdayLunchEnd' AND event_type = 3";
		$sqlGetCurrentNumberOfLeavingEventsRegistered = "SELECT COUNT(*) FROM events WHERE event_time >= '$yesterdayLunchBeginning' AND event_time <= '$yesterdayLunchEnd' AND event_type = 4";
	}
	
	// SQL Query to get the number of times the current user entered in RU
	$sqlGetNumberOfTimesEnteredInRu = "SELECT COUNT(*) FROM events WHERE `target_registry` = '" . $data->registryNumber . "' AND event_type = 3";
	
	// SQL Query to get the credits of the current user
	$sqlGetCredits = "SELECT cellphone_credit, card_credit FROM users WHERE `registry_number` = '" . $data->registryNumber . "'";

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$peopleIn = $conn->query($sqlGetCurrentNumberOfEnteringEventsRegistered);
		// Check connection
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$peopleIn = $peopleIn->fetch_assoc();
			// Execute query
			$peopleOut = $conn->query($sqlGetCurrentNumberOfLeavingEventsRegistered);
			// Check connection
			if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
			else {
				$peopleOut = $peopleOut->fetch_assoc();
				// Write insideRu of output return
				$return["insideRU"] = $peopleIn["COUNT(*)"] - $peopleOut["COUNT(*)"];
				// Execute query
				$result = $conn->query($sqlGetNumberOfTimesEnteredInRu);
				// Check connection
				if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
				else {
					$result = $result->fetch_assoc();
					// Write numberOfEntries of output return
					$return["numberOfEntries"] = $result["COUNT(*)"];
					// Execute query
					$result = $conn->query($sqlGetCredits);
					// Check connection
					if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
					else {
						$result = $result->fetch_assoc();
						// Write cardCredits of output return
						$return["cardCredits"] = $result["card_credit"];
						// Write cellphoneCredits of output return
						$return["cellphoneCredits"] = $result["cellphone_credit"];
					}
				}
			}
		}
	}
	echo json_encode($return); // Return data in json format
	$conn->close(); // Close connection
?>