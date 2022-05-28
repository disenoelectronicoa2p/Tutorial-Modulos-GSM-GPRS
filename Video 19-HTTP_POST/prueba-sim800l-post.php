<?php

$servername = "fdb23.awardspace.net";   
$username = "3420962_sim800l";           
$password = "PE(k1dW}6K:)lyZc";                 
$dbname = "3420962_sim800l";           

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);
// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

$datastring = $_POST["data"];
$latData = substr($datastring,0,7);
$logData = substr($datastring,8,16);

$sql = "INSERT INTO gps_data (latitud, longitud) VALUES ($latData, $logData)";

if (mysqli_query($conn, $sql)) {
    echo "Posicion guardada correctamente...!";
} else {
    echo "Error: " . $sql . "<br>" . mysqli_error($conn);
}

mysqli_close($conn);

?>