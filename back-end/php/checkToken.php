<?php
	include("connection.php");
	
	$data = json_decode(file_get_contents("php://input"));
	$token = $data->token;

	// Connect
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection
	if ($conn->connect_error) echo ("Authentication error - Connection failed: " . $conn->connect_error);
	else {
		$sql = "SELECT tag_number FROM users WHERE web_token='$token'";
		$result = $conn->query($sql);
		if ($conn->error) echo "Authentication error - Server error while consulting database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;
			if ($resultsFound == 1) {
				$result = $result->fetch_assoc();
				$tag_number = $result['tag_number'];
				$sql = "SELECT name, registry_number, tag_number FROM users WHERE tag_number='$tag_number'";
				$result = $conn->query($sql);
				if ($conn->error) echo "Authentication error - Server error while consulting database: " . $conn->error;
				else {
					$result = $result->fetch_assoc();
					echo json_encode($result);
				}
			}
			else {
				echo "Authentication error - User not found";
			}
		}
	}
	$conn->close();
?>