<?php
	include("connection.php");
	$data = json_decode(file_get_contents("php://input"));
	$username = $data->username;
	$password = sha1($data->password);

	$sql = "INSERT INTO users (email, password, token) VALUES ('$username', '$password', 'cacete')";
	$conn->query($sql);

	echo json_encode($username);
?>