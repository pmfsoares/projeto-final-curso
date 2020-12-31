<html>

<head>
    <?php
    if (!isset($_SESSION)) {
        session_start();
    }
    if (isset($_SESSION["username"])) {
        include_once './header.php';
    ?>
        <link rel="stylesheet" href="stylesheet.css">
        <link rel="stylesheet" type="text/css" href="bootstrap/css/bootstrap.min.css">
        <script src="bootstrap/jquery.min.js"></script>
        <script src="angular/angular.min.js"></script>
        <script src="bootstrap/js/bootstrap.min.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js"></script>
        <meta http-equiv="content-type" content="text/html; charset=UTF-8">
</head>

<body>
    <div class='container' ng-app="recordingsApp" ng-controller="recController" id="recordingsApp">
        <script>
            let app = angular.module('recordingsApp', []);
            let s_id = <?php if(isset($_GET["s_id"])){
                                echo $_GET["s_id"];
                            }else echo -666;?>;
            app.controller('recController', function($scope, $interval, $http, $location) {
                $scope.sensor = new Object();
                $scope.mac_list = [];
                $scope.arg;
                $scope.servicoLeitura = function() {
                    $.getJSON(
                        "servicoLeitura/servicoLeituraSensors.php", {
                            arg: $scope.arg,
                            sensor_id: s_id
                        },
                        function(jsonData) {
                            if($scope.arg == 2){
                                $scope.sensor = jsonData[0];
                                $scope.$apply();
                            }else if($scope.arg == 3){
                                for(mac in jsonData){
                                    $scope.mac_list.push(jsonData[mac]);
                                }
                                console.log($scope.mac_list);

                            }
                        }
                    )
                };
                $scope.gen_sensor_mac = function(){
                    let mac = "XX:XX:XX:XX:XX:XX".replace(/X/g, function() {
                        return "0123456789ABCDEF".charAt(Math.floor(Math.random() * 16))
                    });
                    $scope.sensor.sensor_mac = mac;
                }
                $scope.updateSensor = function(sensor_updated) {
                    if(!$scope.checkMac(sensor_updated.sensor_mac)){
                        console.log("Mac error");
                        return;
                    }
                    if(!$scope.checkInteger(sensor_updated.sample_rate)){
                        console.log("Sample rate not an integer");
                        return;
                    }
                    if(!$scope.checkInteger(sensor_updated.sample_size)){
                        console.log("Sample size not an integer");
                        return;
                    }
                    if(!$scope.checkLatLong(sensor_updated.coord_lat, 1)){
                        console.log("Latitute not valid");
                        return
                    }
                    if(!$scope.checkLatLong(sensor_updated.coord_long, 2)){
                        console.log("Longitute not valid");
                        return;
                    }
                    $http.post('sensorUpdate.php', {
                        sensor: sensor_updated
                    }).success(
                        function(data) {
                            $scope.response = data;
                            console.log(data);
                            let str = data.trim();
                                console.log(str);
                                $location.url($location.path() + "/?s_id=" + str);
                        })
                }
                $scope.checkLatLong = function(val, type){
                    //latitute check
                    if(type == 1){
                        let regex_str = /^[-+]?([1-8]?\d(\.\d+)?|90(\.0+)?)$/;
                        return (String(val).match(regex_str) != null ? true : false);
                    
                    //longitude
                    }else if(type == 2){
                        let regex_str = /^[-+]?(180(\.0+)?|((1[0-7]\d)|([1-9]?\d))(\.\d+)?)$/;
                        return (String(val).match(regex_str) != null ? true : false);
                    }
                }
                $scope.checkInteger = function(str){
                    let regex_str = /^[0-9]*$/;
                    return(String(str).match(regex_str) != null ? true : false);
                }
                $scope.checkMac = function(str){
                    let regex_str = /^([0-9A-F]{2}[:]){5}([0-9A-F]{2})$/;
                    return (String(str).match(regex_str) != null ? true : false);
                }
            
                if(s_id > 0){
                    $scope.arg = 2;
                    $scope.servicoLeitura();
                }else if(s_id == -666){
                    $scope.arg = 3;
                    $scope.servicoLeitura();
                    console.log("Called read service");
                    $scope.gen_sensor_mac();
                }
            });
        </script>
        <div class="container">
            <div class="row">
                <div class="col-lg-3 col-md-2"></div>
                <div class="col-lg-6 col-md-8 login-box">
                    <div class="col-lg-12 login-title">
                        Sensor Config
                    </div>
                    <div class="col-lg-12 login-form">
                        <div class="col-lg-12 login-form">
                            <form method="post">
                                <div class="form-row">
                                    <div class="col-md-5">
                                        <label class="form-control-label">Sensor MAC</label>
                                        <input type="text" class="form-control" ng-model="sensor.sensor_mac">
                                    </div>
                                    <div class="col-md-1">
                                        <button class="btn" ng-click="gen_sensor_mac()">
                                            <span class="glyphicon glyphicon-refresh"></span>
                                        </button>
                                    </div>
                                    <div class="col-md-6">
                                        <label class="form-control-label">Sensor Descriptor</label>
                                        <input type="text" class="form-control" ng-model="sensor.sensor_descriptor">
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="form-control-label">Model</label>
                                    <input type="text" class="form-control" ng-model="sensor.model">
                                </div>

                                <div class="form-row">
                                    <div class="col-md-6">
                                        <label class="form-control-label">Latitude</label>
                                        <input type="text" class="form-control" ng-model="sensor.coord_lat">
                                    </div>
                                    <div class="col-md-6">
                                        <label class="form-control-label">Longitude</label>
                                        <input type="text" class="form-control" ng-model="sensor.coord_long">
                                    </div>
                                </div>

                                <div class="form-row">
                                    <div class="col-md-6">
                                        <label class="form-control-label">Sample Rate</label>
                                        <input type="text" class="form-control" ng-model="sensor.sample_rate">
                                    </div>
                                    <div class="col-md-6">
                                        <label class="form-control-label">Sample Size</label>
                                        <input type="text" class="form-control" ng-model="sensor.sample_size">
                                    </div>
                                </div>
                                <div class="col-lg-12 loginbttm">
                                    <div class="col-6 login-btm login-button">
                                        <button type="submit" ng-click="updateSensor(sensor)" class="btn btn-outline-login float-right">Submit</button>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <p><?php
                                        if (isset($_SESSION["msg_err"])) {
                                            echo $_SESSION["msg_err"];
                                            unset($_SESSION["msg_err"]);
                                        }
                                        ?>
                                    </p>
                                </div>
                            </form>
                        </div>
                    </div>
                </div>
            </div>
        </div>
</body>

</html>
<?php } else {
        include 'index.php';
    }
?>