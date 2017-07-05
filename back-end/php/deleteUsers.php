<?php
	// Include database parameters
	include("connection.php");

	// Read post data
	$data = json_decode(file_get_contents("php://input"));
	
	$usersToDelete = $data->usersToDelete;
	$sourceAdmin = $data->sourceAdmin;

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection
	if ($conn->connect_error) echo ("Error - Authentication error - Connection failed: " . $conn->connect_error);
	else {
		$i = 0;
		$sqlAdapt = " `registry_number` = '";
		// Mount query with all the users to be deleted
		while ($i < sizeof($usersToDelete)) {
			$sqlAdapt = $sqlAdapt . $usersToDelete[$i];
			if ($i != (sizeof($usersToDelete)-1)) $sqlAdapt = $sqlAdapt . "' OR `registry_number` = '";
			else $sqlAdapt = $sqlAdapt . "'";
			$i = $i + 1;
		}
		// SQL query used to load information of the user who is going to be deleted in order to store his basic information in a stack
		$sqlSelectUser = "SELECT tag_number, registry_number FROM users WHERE" . $sqlAdapt;
		// SQL query used to delete the user from the users table
		$sqlDeleteUser = "DELETE FROM users WHERE" . $sqlAdapt;
		
		// Execute query
		$result = $conn->query($sqlSelectUser);
		// Check connection
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;
			// Check if there are results to be added to the users_buff table
			if ($resultsFound > 0) {
				$i = 0;
				// SQL query used to store deleted user information (it is used by the board which only stores the tag number and not the registry number)
				$insertionQuery = "INSERT INTO users_buff (`tag_number`, `registry_number`) VALUES ";
				// Mount query with all the users deleted
				while($row = $result->fetch_assoc()) {
					$registryNumber = $row["registry_number"];
					$tagNumber = $row["tag_number"];
					$insertionQuery .= "(" . $tagNumber . ", '" . $registryNumber . "')";
					if ($i != ($resultsFound - 1)) $insertionQuery .= ", ";
					$i = $i + 1;
				}
				
				// Execute query
				$conn->query($insertionQuery);
				// Check connection
				if ($conn->error) echo "Error - Server error while updating database: " . $conn->error . $insertionQuery;
				else {
					// Execute query
					$conn->query($sqlDeleteUser);
					// Check connection
					if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
					else {
						$i = 0;
						$deletedUsers = " ";
						// Mount query to insert into events table the deletion of each user
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
			}
			else {
				echo "Error - No user found";
			}
		}
	}
	$conn->close(); // Close connection
?>