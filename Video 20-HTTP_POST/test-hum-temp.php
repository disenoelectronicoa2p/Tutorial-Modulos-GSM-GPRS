<?php

$servername = "fdb23.awardspace.net";   
$username = "username";           
$password = "password";                 
$dbname = "dbname";           

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);
// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

$datastring = $_POST["data"];
$temData = substr($datastring,0,5);
$humData = substr($datastring,6,10);

$sql = "INSERT INTO humidity_temperature (temperatura, humedad) VALUES ($temData, $humData)";

if (mysqli_query($conn, $sql)) {
    echo "Humedad y temperarura guardada correctamente...!";
} else {
    echo "Error: " . $sql . "<br>" . mysqli_error($conn);
}

mysqli_close($conn);

?>