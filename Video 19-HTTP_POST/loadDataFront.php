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

$sql = "SELECT * from gps_data ORDER BY id ASC";

$result = mysqli_query($conn, $sql);

if (mysqli_num_rows($result) > 0) {
    // output data of each row
    while($row = mysqli_fetch_assoc($result)) {
        echo "id: " . $row["id"]. " - latitud: " . $row["latitud"]. " -longitud: " . $row["longitud"]. "<br>";
    }
} else {
    echo "0 results";
}

mysqli_close($conn);
?>