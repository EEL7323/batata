<?php
	include("connection.php");

	//$data = json_decode(file_get_contents("php://input"));

	$data->RegistryNumber = "13200964";

	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	$sql = "INSERT INTO events (`source_registry`, `target_registry`, `event_type`, `diff_cred_cellphone`, `diff_cred_tag`, `to_resolve`) VALUES ('board', '$data->RegistryNumber', 4, 0, 0, 0)";

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		$conn->query($sql);
		if ($conn->error) echo "Error - Server error while updating database: " . $conn->error;
		else {
			echo "Update Successful!";
		}
	}
	$conn->close();
?>