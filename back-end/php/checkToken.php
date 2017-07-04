<?php
	// Include database parameters
	include("connection.php");
	
	// Read post data
	$data = json_decode(file_get_contents("php://input"));
	$token = $data->token;

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection
	if ($conn->connect_error) echo ("Authentication error - Connection failed: " . $conn->connect_error);
	else {
		// SQL query used to check if there is a user with that token
		$sql = "SELECT tag_number FROM users WHERE web_token='$token'";
		// Execute query
		$result = $conn->query($sql);
		// Check connection
		if ($conn->error) echo "Authentication error - Server error while consulting database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;
			// Check if there is one and only one user
			if ($resultsFound == 1) {
				$result = $result->fetch_assoc();
				$tag_number = $result['tag_number'];
				// SQL query used to load data from the user which token was checked before
				$sql = "SELECT name, registry_number, tag_number FROM users WHERE tag_number='$tag_number'";
				// Execute query
				$result = $conn->query($sql);
				// Check connection
				if ($conn->error) echo "Authentication error - Server error while consulting database: " . $conn->error;
				else {
					$result = $result->fetch_assoc();
					// Return the result
					echo json_encode($result);
				}
			}
			else {
				echo "Authentication error - User not found";
			}
		}
	}
	$conn->close(); // Close connection
?>