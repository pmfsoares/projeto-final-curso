<?php
header("Content-type: application/json");

include '../mysql/mysqlConnect.php';

session_start();
if(isset($_GET["arg"])){
    $arg = $_GET["arg"];

    //post por perfil
    if($arg == 1) {
        $query = "SELECT s.id, s.sensor_descriptor, s.sensor_mac, s.model, s.coord_long, s.coord_lat, s.sample_rate, s.sample_size, s.status as status from sensors as s join status_type as st where s.status = st.id";

    }else if($arg == 2){
        $sensor_id = $_GET["sensor_id"];
        $query = "SELECT s.id, s.sensor_descriptor, s.sensor_mac, s.model, s.coord_long, s.coord_lat, s.sample_rate, s.sample_size from sensors as s where s.id = '" . $sensor_id . "'";
    }else if($arg == 3){
        $query = "SELECT s.sensor_mac from sensors as s";
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
