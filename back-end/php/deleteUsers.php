<?php
	include("connection.php");
	$data = json_decode(file_get_contents("php://input"));
	
	$usersToDelete = $data->usersToDelete;
	$sourceAdmin = $data->sourceAdmin;

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	if ($conn->connect_error) echo ("Error - Authentication error - Connection failed: " . $conn->connect_error);
	else {
		$i = 0;
		$sqlAdapt = " registry_number = '";
		while ($i < sizeof($usersToDelete)) {
			$sqlAdapt = $sqlAdapt . $usersToDelete[$i];
			if ($i != (sizeof($usersToDelete)-1)) $sqlAdapt = $sqlAdapt . "' OR registry_number = '";
			else $sqlAdapt = $sqlAdapt . "'";
			$i = $i + 1;
		}

		$sqlDeleteUser = "DELETE FROM users WHERE" . $sqlAdapt;
		$conn->query($sqlDeleteUser);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			$i = 0;
			$deletedUsers = " ";
			while ($i < sizeof($usersToDelete)) {
				$sqlDeleteUserEvent = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('$sourceAdmin', '$usersToDelete[$i]', 1, 0, 0, 1)";
				$conn->query($sqlDeleteUserEvent);
				if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
				else {
				$deletedUsers .= $usersToDelete[$i] . " ";
				}
				$i = $i + 1;
			}
			echo ("Delete Successful: {" . $deletedUsers . "}");
		}
	}
	$conn->close();
?>