<?php

include '../mysql/mysqlConnect.php';

$id = _GET["id"];

$query = "SELECT data FROM recordings where id = $id";

$result = $GLOBALS["db.connection"]->query($query);
if($result->num_rows > 0 ) {
    $row = $result->fetch_assoc();
    
    header('Content-Description: File Transfer');
    header("Content-Transfer-Encoding: binary");
    header('Accept-Ranges: bytes');
    header("Content-Type: audio/wav"); 
    header('Content-Disposition: inline; filename="1.wav"');
    echo $row['data'];

    $result->free();
}

include '../mysql/mysqlClose.php';
?>