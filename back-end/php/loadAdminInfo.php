<?php
	include("connection.php");

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	date_default_timezone_set('Brazil/East');

	// Today RU times

	$todayLunchBeginning = date('Y-m-d H:i:s', mktime(11, 0, 0, date("m"), date("d"), date("Y")));
	$todayLunchEnd = date('Y-m-d H:i:s', mktime(14, 0, 0, date("m"), date("d"), date("Y"))); // 13:30 + 30 minutes ~estimate
	$todayDinnerBeginning = date('Y-m-d H:i:s', mktime(17, 0, 0, date("m"), date("d"), date("Y")));
	$todayDinnerEnd = date('Y-m-d H:i:s', mktime(19, 30, 0, date("m"), date("d"), date("Y"))); // 19:00 + 30 minutes ~estimate

	// Yesterday RU times

	$yesterdayLunchBeginning = date('Y-m-d H:i:s', mktime(11, 0, 0, date("m"), date("d")-1, date("Y")));
	$yesterdayLunchEnd = date('Y-m-d H:i:s', mktime(14, 0, 0, date("m"), date("d")-1, date("Y"))); // 13:30 + 30 minutes ~estimate
	$yesterdayDinnerBeginning = date('Y-m-d H:i:s', mktime(17, 0, 0, date("m"), date("d")-1, date("Y")));
	$yesterdayDinnerEnd = date('Y-m-d H:i:s', mktime(19, 30, 0, date("m"), date("d")-1, date("Y"))); // 19:00 + 30 minutes ~estimate

	// Queries to get desired data

	if (date('H:i:s') > date('H:i:s', mktime(15, 0, 0))) {
		$sqlGetCurrentNumberOfEnteringEventsRegistered = "SELECT COUNT(*) FROM events WHERE event_time >= '$todayDinnerBeginning' AND event_time <= '$todayDinnerEnd' AND event_type = 3";
		$sqlGetCurrentNumberOfLeavingEventsRegistered = "SELECT COUNT(*) FROM events WHERE event_time >= '$todayDinnerBeginning' AND event_time <= '$todayDinnerEnd' AND event_type = 4";
	}
	else {
		$sqlGetCurrentNumberOfEnteringEventsRegistered = "SELECT COUNT(*) FROM events WHERE event_time >= '$yesterdayLunchBeginning' AND event_time <= '$yesterdayLunchEnd' AND event_type = 3";
		$sqlGetCurrentNumberOfLeavingEventsRegistered = "SELECT COUNT(*) FROM events WHERE event_time >= '$yesterdayLunchBeginning' AND event_time <= '$yesterdayLunchEnd' AND event_type = 4";
	}

	$sqlGetNumberOfStudents = "SELECT COUNT(*) FROM users WHERE access_level = 1";
	$sqlGetNumberOfPeopleInLuchYesterday = "SELECT COUNT(*) FROM events WHERE event_time >= '$yesterdayLunchBeginning' AND event_time <= '$yesterdayLunchEnd' AND event_type = 3";
	$sqlGetNumberOfPeopleInDinnerYesterday = "SELECT COUNT(*) FROM events WHERE event_time >= '$yesterdayDinnerBeginning' AND event_time <= '$yesterdayDinnerEnd' AND event_type = 3";

	// Check connection

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		$result = $conn->query($sqlGetNumberOfStudents);
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$result = $result->fetch_assoc();
			$return["subscribedStudents"] = $result["COUNT(*)"];
			$peopleIn = $conn->query($sqlGetCurrentNumberOfEnteringEventsRegistered);
			$peopleIn = $peopleIn->fetch_assoc();
			if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
			else {
				$peopleOut = $conn->query($sqlGetCurrentNumberOfLeavingEventsRegistered);
				$peopleOut = $peopleOut->fetch_assoc();
				$return["insideRU"] = $peopleIn["COUNT(*)"] - $peopleOut["COUNT(*)"];
				$result = $conn->query($sqlGetNumberOfPeopleInLuchYesterday);
				if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
				else {
					$result = $result->fetch_assoc();
					$return["lastLunch"] = $result["COUNT(*)"];
					$result = $conn->query($sqlGetNumberOfPeopleInDinnerYesterday);
					if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
					else {
						$result = $result->fetch_assoc();
						$return["lastDinner"] = $result["COUNT(*)"];
					}
				}
			}
		}
	}
	echo json_encode($return);
	$conn->close();
?>