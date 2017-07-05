<?php
	// Include database parameters
	include("connection.php");

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

	// SQL query to get the number of students registered in the system
	$sqlGetNumberOfStudents = "SELECT COUNT(*) FROM users WHERE access_level = 1";
	
	// SQL queries to get the number of people who ate at RU yesterday in lunch and dinner time
	$sqlGetNumberOfPeopleInLuchYesterday = "SELECT COUNT(*) FROM events WHERE event_time >= '$yesterdayLunchBeginning' AND event_time <= '$yesterdayLunchEnd' AND event_type = 3";
	$sqlGetNumberOfPeopleInDinnerYesterday = "SELECT COUNT(*) FROM events WHERE event_time >= '$yesterdayDinnerBeginning' AND event_time <= '$yesterdayDinnerEnd' AND event_type = 3";

	// Check connection
	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		// Execute query
		$result = $conn->query($sqlGetNumberOfStudents);
		// Check connection
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$result = $result->fetch_assoc();
			// Write subscribedStudents of output return
			$return["subscribedStudents"] = $result["COUNT(*)"];
			// Execute query
			$peopleIn = $conn->query($sqlGetCurrentNumberOfEnteringEventsRegistered);
			$peopleIn = $peopleIn->fetch_assoc();
			// Check connection
			if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
			else {
				// Execute query
				$peopleOut = $conn->query($sqlGetCurrentNumberOfLeavingEventsRegistered);
				if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
				else {
					$peopleOut = $peopleOut->fetch_assoc();
					// Write insideRu of output return
					$return["insideRU"] = $peopleIn["COUNT(*)"] - $peopleOut["COUNT(*)"];
					// Execute query
					$result = $conn->query($sqlGetNumberOfPeopleInLuchYesterday);
					// Check connection
					if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
					else {
						$result = $result->fetch_assoc();
						// Write lastLunch of output return
						$return["lastLunch"] = $result["COUNT(*)"];
						// Execute query
						$result = $conn->query($sqlGetNumberOfPeopleInDinnerYesterday);
						// Check connection
						if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
						else {
							$result = $result->fetch_assoc();
							// Write lastDinner of output return
							$return["lastDinner"] = $result["COUNT(*)"];
						}
					}
				}
			}
		}
	}
	echo json_encode($return); // Return data in json format
	$conn->close(); // Close connection
?>