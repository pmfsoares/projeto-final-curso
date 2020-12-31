<?php


include './mysql/mysqlConnect.php';

$postdata = file_get_contents('php://input');
$postdate = utf8_encode($postdata);
$request = json_decode($postdata, true);
$sensor = $request["sensor"];

if(isset($sensor['id'])){
    echo $sensor["sensor_id"];
    $update_str_start = "UPDATE sensors SET ";
    $update_str_end = "' WHERE ( id = '" . $sensor["id"] . "');";
    $update_sql = 
    $update_str_start   . " sensor_mac   = '" . $sensor["sensor_mac"]
                        . "' , sensor_descriptor    = '" . $sensor["sensor_descriptor"]
                        . "' , model        = '" . $sensor["model"]
                        . "' , coord_lat    = '" . $sensor["coord_lat"]
                        . "' , coord_lat    = '" . $sensor["coord_lat"]
                        . "' , sample_rate  = '" . $sensor["sample_rate"]
                        . "' , sample_size  = '" . $sensor["sample_size"]
                        . $update_str_end;
    $GLOBALS["db.connection"]->query($update_sql);
    echo json_encode($GLOBALS["db.connection"]->error);
    include './mysql/mysqlClose.php';
    echo $sensor->id;
    
    //echo $GLOBALS["db.connection"]->error;
}else{
    $insert_sql = "INSERT INTO `projeto`.`sensors` (`sensor_mac`, `sensor_descriptor`, `model`, `coord_long`, `coord_lat`, `sample_rate`, `sample_size`, `status`) VALUES ('" . $sensor["sensor_mac"] . "', '" . $sensor["sensor_descriptor"] . "', '" . $sensor["model"] . "', '" . $sensor["coord_long"] . "', '" . $sensor["coord_lat"] . "', '" . $sensor["sample_rate"] . "', '" . $sensor["sample_size"] . "', '2' );";
    $GLOBALS["db.connection"]->query($insert_sql);
    $tmp_id = mysqli_insert_id($GLOBALS["db.connection"]);
    include './mysql/mysqlClose.php';
    echo $tmp_id;
}