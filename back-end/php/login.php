<?php
	include("connection.php");
	include("randomGenerator.php");

	$data = json_decode(file_get_contents("php://input"));

	$userRegistry = $data->registry;
	$userPassword = sha1($data->password);

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	$sql = "SELECT tag_number, access_level FROM users WHERE registry_number='$userRegistry' AND password='$userPassword'";

	// Try to connect
	if ($conn->connect_error) echo("Authentication error - Connection failed: " . $conn->error);
	else {
		// Read results
		$result = $conn->query($sql);
		$resultsFound = $result->num_rows;
		if ($resultsFound == 1) {
			$result = $result->fetch_assoc();
			$token = random(7) . $result["access_level"] . random(8) . " | " . $userRegistry;
			$tag_number = $result['tag_number'];
			$sql = "UPDATE users SET web_token='$token' WHERE tag_number='$tag_number'";
			$conn->query($sql);
			if ($conn->error) echo "Authentication error - Server error while updating user: " . $conn->error;
			else echo $token;
		}
		else {
			echo "Authentication error - User not found";
		}
	}
	$conn->close();	
?>