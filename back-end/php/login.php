<?php
	// Include database parameters
	include("connection.php");

	// Include random generator function
	include("randomGenerator.php");

	// Read post data
	$data = json_decode(file_get_contents("php://input"));

	$userRegistry = $data->registry;
	$fromApp = $data->fromApp;

	// Convert the password to sha1 (hash function)
	$userPassword = sha1($data->password);
	
	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// SQL query to load the user with registry number and access level passed as parameter in the post request
	$sql = "SELECT tag_number, access_level FROM users WHERE registry_number='$userRegistry' AND password='$userPassword'";

	// Check connection
	if ($conn->connect_error) echo("Authentication error - Connection failed: " . $conn->error);
	else {
		// Execute query
		$result = $conn->query($sql);
		$resultsFound = $result->num_rows;
		// Check if there is one and only one user for that registry number and password combination
		if ($resultsFound == 1) {
			if ($fromApp == 0) {
				$result = $result->fetch_assoc();
				$token = random(7) . $result["access_level"] . random(8) . " | " . $userRegistry;
				$tag_number = $result['tag_number'];
				// SQL query to set a new token to the now logged user
				$sql = "UPDATE users SET web_token='$token' WHERE tag_number='$tag_number'";
				// Execute query
				$conn->query($sql);
				// Check connection
				if ($conn->error) echo "Authentication error - Server error while updating user: " . $conn->error;
				else echo $token;
			}
			else echo "Login Successful";
		}
		else {
			echo "Authentication error - User not found";
		}
	}
	$conn->close();	// Close connection
?>