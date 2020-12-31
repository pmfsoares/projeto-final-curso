<?php
header("Content-type: application/json");

include '../mysql/mysqlConnect.php';

session_start();
if(isset($_GET["arg"])){
    $arg = $_GET["arg"];
    $maxId = $_GET["maxId"];
    $minId = $_GET["minId"];
    $lim = $_GET["limit"];
    //post por perfil
    if($arg == 1) {
        $query = "SELECT r.id, r.sensor_mac, s.id as id_sensor_t, r.timestamp, r.sample_rate, r.sample_size, r.filename, r.data from recordings as r join sensors as s where r.sensor_mac = s.sensor_mac and r.id > '" . $maxId . "' order by r.timestamp desc limit " . $lim . ";";

    }else if($arg == 2){
        $query = "SELECT r.id, r.sensor_mac, s.id as id_sensor_t, r.timestamp, r.sample_rate, r.sample_size, r.filename, r.data from recordings as r join sensors as s where r.sensor_mac = s.sensor_mac and r.id < '" . $minId . "' order by r.timestamp desc limit " . $lim . ";";
    }
}

$result = $GLOBALS["db.connection"]->query($query);

$todos = array();
while ($row = $result->fetch_assoc()) {
    $todos[] = $row; //atribui o array do user à ultima prosicao do array geral
}
echo json_encode($todos);

include '../mysql/mysqlClose.php';

?>
