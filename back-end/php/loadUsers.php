<?php
	include("connection.php");

	$data = json_decode(file_get_contents("php://input"));
	
	if ($data->registryNumber == "all") $sql = "SELECT name, registry_number, tag_number, cellphone_credit, card_credit, access_level FROM users";
	else $sql = "SELECT name, registry_number, tag_number, cellphone_credit, card_credit, access_level FROM users WHERE `registry_number` = " . $data->registryNumber;
	
	$conn = new mysqli($serverName, $userName, $password, $dbName);

	// Check connection

	if ($conn->connect_error) echo ("Error - Connection failed: " . $conn->connect_error);
	else {
		
		$result = $conn->query($sql);
		if ($conn->error) echo "Error - Server error while consulting database: " . $conn->error;
		else {
			$resultsFound = $result->num_rows;

			if ($resultsFound > 0) {
				$i = 0;
				echo "[";
				while($row = $result->fetch_assoc()) {
				$return["name"] = $row["name"];
				$return["registryNumber"] = $row["registry_number"];
				$return["tagNumber"] = $row["tag_number"];
				$return["cellphoneCredit"] = $row["cellphone_credit"];
				$return["cardCredit"] = $row["card_credit"];
				$return["accessLevel"] = $row["access_level"];
				$return["id"] = $i;
				$i = $i + 1;
				echo json_encode($return);
				if ($i != ($resultsFound)) echo ",";
				}
				echo "]";
			}
			else {
				echo "Error - No user found";
			}
		}
	}
	$conn->close();
?>