<?php

// Server Parameters
$serverName = "localhost";
$userName = "root";
$password = "";
$dbName = "ru_server";

// Create first connection
$conn = new mysqli($serverName, $userName, $password);

// Check connection
if ($conn->connect_error) {
    echo ("Connection failed: " . $conn->connect_error);
} 