<?php
	include("connection.php");
	$data = json_decode(file_get_contents("php://input"));
	$token = $data->token;

	$sql = "UPDATE users SET token='invalid' WHERE token='$token'";
	$conn->query($sql);
?>