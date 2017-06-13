<?php
	include("connection.php");

	$data = json_decode(file_get_contents("php://input"));

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

	$sqlGetCurrentNumberOfEnteringEventsRegistered = "SELECT COUNT(*) FROM events WHERE `event_time` >= '$todayLunchBeginning' AND `event_time` <= '$todayLunchEnd' AND `event_type` = 3";
	$sqlGetCurrentNumberOfLeavingEventsRegistered = "SELECT COUNT(*) FROM events WHERE `event_time` >= '$todayLunchBeginning' AND `event_time` <= '$todayLunchEnd' AND `event_type` = 4";
	
	$sqlGetNumberOfTimesEnteredInRu = "SELECT COUNT(*) FROM events WHERE `target_registry` = '" . $data->registryNumber . "' AND event_type = 3";
	
	$sqlGetCredits = "SELECT cellphone_credit, card_credit FROM users WHERE `registry_number` = '" . $data->registryNumber . "'";

	// Check connection

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		$peopleIn = $conn->query($sqlGetCurrentNumberOfEnteringEventsRegistered);
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$peopleIn = $peopleIn->fetch_assoc();
			$peopleOut = $conn->query($sqlGetCurrentNumberOfLeavingEventsRegistered);
			if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
			else {
				$peopleOut = $peopleOut->fetch_assoc();
				$return["insideRU"] = $peopleIn["COUNT(*)"] - $peopleOut["COUNT(*)"];
				$result = $conn->query($sqlGetNumberOfTimesEnteredInRu);
				if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
				else {
					$result = $result->fetch_assoc();
					$return["numberOfEntries"] = $result["COUNT(*)"];
					$result = $conn->query($sqlGetCredits);
					if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
					else {
						$result = $result->fetch_assoc();
						$return["cardCredits"] = $result["card_credit"];
						$return["cellphoneCredits"] = $result["cellphone_credit"];
					}
				}
			}
		}
	}
	echo json_encode($return);
	$conn->close();
?>